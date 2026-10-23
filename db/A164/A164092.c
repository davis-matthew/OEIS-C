/*
 * A164092  Triangle by 2^n term rows, codes used for generating Petoukhov
 *          matrices in a Gray code format.
 * https://oeis.org/A164092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 46, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 0
 *   For n > 0, let r = ⌊log₂(n+1)⌋ and p = n - (2ʳ - 1)  (position within its row).
 *   Then
 *      if p < 2ᵇ⁻¹   → a(n) = a(row r-1, p) + 1
 *      else          → a(n) = a(row r-1, 2ᵇ - 1 - p) - 1
 *   The implementation follows this rule iteratively. */
seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t t = n + 1;
    /* compute floor(log2(n+1)) safely for all n < 2⁶⁴ */
    while (t >> 1) {
        ++r;
        t >>= 1;
    }
    idx_t start = ((idx_t)1 << r) - 1;      /* first index of row r */
    idx_t p = n - start;                    /* position within row r */

    seq_t val = 0;
    while (r > 0) {
        if (p < ((idx_t)1 << (r - 1))) {
            /* first half of the row: value is previous + 1 */
            val += 1;
        } else {
            /* second half: value is previous - 1, reversed index */
            val -= 1;
            p = ((idx_t)1 << r) - 1 - p;
        }
        --r;
    }
    return val;
}
