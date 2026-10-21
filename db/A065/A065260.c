/*
 * A065260  A057115 conjugated with A059893, inverse of A065259.
 * https://oeis.org/A065260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n for even n; a(n) = n+1 for n ≡ 1 (mod 4); a(n) =
 *            (n-1)/2 for n ≡ 3 (mod 4)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: n >= 1 */
    /* Even indices */
    if ((n & 1ULL) == 0) 
        return (seq_t)n * 2;

    /* Odd indices: distinguish mod 4 */
    if ((n & 3ULL) == 1)            /* n = 4k+1 */
        return (seq_t)n + 1;

    /* Remaining odd case: n = 4k+3 */
    return ((seq_t)n - 1) / 2;
}
