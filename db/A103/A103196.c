/*
 * A103196  a(n) = (1/9)(2^(n+3)-(-1)^n(3n-1)).
 * https://oeis.org/A103196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+3) - (-1)^n (3n-1)) / 9
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Comprehensive formula from the OEIS entry for A103196. */
seq_t a(idx_t n) {
    /* Compute 2^(n+3) as a seq_t value. */
    seq_t pow = 1;
    idx_t exp = n + 3;
    for (idx_t i = 0; i < exp; ++i) {
        pow *= 2;
    }

    /* Compute t = (-1)^n * (3n - 1) as a signed 64‑bit integer. */
    int64_t term = (int64_t)(3 * n - 1);
    if (n & 1) {          /* n is odd: (-1)^n = -1 */
        term = -term;
    }

    /* Subtract t from pow, handling possible negative t. */
    if (term >= 0) {
        pow -= (seq_t)term;
    } else {
        pow += (seq_t)(-term);
    }

    /* Divide by 9.  The division is exact for all n. */
    return pow / 9;
}
