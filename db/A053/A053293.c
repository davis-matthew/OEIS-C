/*
 * A053293  Number of nonsingular n X n matrices over GF(7).
 * https://oeis.org/A053293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (7^n – 7^k) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t pow7n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow7n *= (seq_t)7;          /* compute 7^n */

    seq_t result = 1;
    seq_t pow7k  = 1;                /* 7^k, starting with k=0 */

    for (idx_t k = 0; k < n; ++k) {
        result *= (pow7n - pow7k);   /* multiply by (7^n – 7^k) */
        pow7k *= (seq_t)7;           /* advance to next power of 7 */
    }

    return result;
}
