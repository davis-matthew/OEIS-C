/*
 * A028685  Galois numbers for p=23; order of group AGL(n,23).
 * https://oeis.org/A028685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23^n * Product_{k=0..n-1} (23^n - 23^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 23;
    seq_t p_pow_n = 1;
    for (idx_t i = 0; i < n; ++i)
        p_pow_n *= p;
    seq_t result = 1;
    seq_t pow_k = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        result *= (p_pow_n - pow_k);
        pow_k *= p;
    }
    result *= p_pow_n;
    return result;
}
