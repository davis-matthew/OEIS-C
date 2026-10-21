/*
 * A065128  Number of invertible n X n matrices mod 4 (i.e., over the ring
 *          Z_4).
 * https://oeis.org/A065128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n^2) * Product_{k=1..n} (1 - 1/2^k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = ((seq_t)1 << k) - 1;   /* 2^k - 1 */
        result *= term;
    }
    idx_t exp = n * (3 * n - 1) / 2;        /* exponent of 2: n(3n‑1)/2 */
    result <<= exp;                         /* multiply by 2^exp */
    return result;
}
