/*
 * A165933  Least integer, k, whose value is n in A165413.
 * https://oeis.org/A165933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t ones_mask(idx_t len) {
    seq_t m = 0;
    while (len) {
        idx_t k = (len < 63) ? len : 63;
        m <<= k;
        m |= ((seq_t)1 << k) - 1;
        len -= k;
    }
    return m;
}

static void shift_left_by(seq_t *x, idx_t count) {
    while (count) {
        idx_t k = (count < 63) ? count : 63;   /* safe shift step */
        *x <<= k;
        count -= k;
    }
}
/* Sequence A165933: binary pattern with alternating runs of 1s and 0s,
   starting with a single 1, followed by n zeros, then 2 ones, (n-1) zeros,
   3 ones, … until a total of n runs have been produced. */

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1 */
    seq_t result = 1;       /* first run: single 1 */

    for (idx_t r = 2; r <= n; ++r) {
        if ((r & 1U) == 0) {           /* even run: zeros */
            idx_t zeros_len = n - (r / 2) + 1;
            shift_left_by(&result, zeros_len);
        } else {                        /* odd run (>=3): ones */
            idx_t ones_len = (r + 1) / 2;
            shift_left_by(&result, ones_len);
            result |= ones_mask(ones_len);
        }
    }
    return result;
}
