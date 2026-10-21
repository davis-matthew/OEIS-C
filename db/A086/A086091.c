/*
 * A086091  a(n) = 3^n + 3*n*4^(n-1).
 * https://oeis.org/A086091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 3*n*4^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return 1;

    seq_t pow3 = 1;           /* will become 3^n */
    seq_t pow4_n_minus1 = 1;  /* will become 4^(n-1) */

    /* Compute 3^(n-1) and 4^(n-1) in one loop */
    for (idx_t i = 0; i < n - 1; ++i) {
        pow3 *= 3;
        pow4_n_minus1 *= 4;
    }

    /* Finish the 3^n term */
    pow3 *= 3;

    /* a(n) = 3^n + 3*n*4^(n-1) */
    seq_t term = pow3 + 3 * (seq_t)n * pow4_n_minus1;
    return term;
}
