/*
 * A135560  a(n) = A007814(n) + A036987(n-1) + 1.
 * https://oeis.org/A135560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007814(n) + A036987(n-1) + 1 Simplified implementation:
 *            - a(1) = 2 - if n is odd then a(n) = 1 - else let t = number of
 *            trailing zeros of n if n is a power of two then a(n) = t + 2
 *            else a(n) = t + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)2;         /* special case n = 1 */
    if (n & 1ULL)       return (seq_t)1;   /* odd n > 1 */

    /* even n: compute t = ctz(n) */
    unsigned t = __builtin_ctzll((unsigned long long)n);

    /* check whether n is a power of two */
    if (((idx_t)1ULL << t) == n)
        return (seq_t)(t + 2);            /* power of two: a(n) = t + 2 */
    else
        return (seq_t)(t + 1);            /* otherwise: a(n) = t + 1 */
}
