/*
 * A065171  Permutation of Z, folded to N, corresponding to the site swap
 *          pattern ...26120123456... which ascends infinitely after t=0.
 * https://oeis.org/A065171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 2*n if n is even, a(n) = (n+1)/2 if n ≡ 1 (mod
 *            4), a(n) = n-1 if n ≡ 3 (mod 4)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;
    if ((n & 1u) == 0u)          /* even */
        return (seq_t)((seq_t)2 * n);
    if ((n & 3u) == 1u)          /* n % 4 == 1 */
        return (seq_t)((seq_t)(n + 1) / 2);
    /* n % 4 == 3 */
    return (seq_t)(n - 1);
}
