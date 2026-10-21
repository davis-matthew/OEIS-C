/*
 * A052497  Number of nonsingular n X n matrices over GF(9).
 * https://oeis.org/A052497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (9^n - 9^k)
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

    /* compute 9^n */
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow9 *= 9;

    /* compute product ∏_{k=0}^{n-1} (9^n - 9^k) */
    seq_t result = 1;
    seq_t pow9k = 1;           /* 9^k */
    for (idx_t k = 0; k < n; ++k)
    {
        result *= (pow9 - pow9k);
        pow9k *= 9;
    }

    return result;
}
