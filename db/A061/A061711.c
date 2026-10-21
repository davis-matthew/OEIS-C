/*
 * A061711  a(n) = n^n * n!.
 * https://oeis.org/A061711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n * n! (with the convention a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;                      /* a(0) = 1 */

    /* Compute the product prod_{k=1..n} (k * n) = n^n * n! */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)k * (seq_t)n;  /* k*n in seq_t arithmetic */
        result *= term;                    /* multiply into the running product */
    }

    return result;
}
