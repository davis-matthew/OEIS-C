/*
 * A087734  a(n) = f(f(n)), where f() = A035327().
 * https://oeis.org/A087734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(floor(n/2)) + (n mod 2) - [n + 1 is a power of two]
 *            with * a(0) = a(1) = 0. The term in brackets equals 1 iff n+1 is
 *            a power of two * (i.e., n = 2^k - 1). This recurrence follows
 *            from the definition * a(n) = f(f(n)) where f = A035327 and the
 *            auxiliary sequence * A036987(n) = isPowerOfT...
 * Range:     u8 n <= 1279, u16 n <= 8835, u32 n <= 8835, u64 n <= 8835,
 *            u128 n <= 8835, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases : a(0) = a(1) = 0 */
    if (n <= 1)
        return 0;

    /* Recursive computation using the recurrence above */
    seq_t half = a(n >> 1);                 /* a(floor(n/2))          */
    seq_t res  = (half << 1) | (seq_t)(n & 1); /* 2*half + (n mod 2)  */

    /* Subtract 1 if n+1 is a power of two */
    idx_t x = n + 1;                      /* safe for n <= UINT64_MAX-1 */
    /* A power of two satisfies x > 0 && (x & (x-1)) == 0 */
    if (x != 0 && (x & (x - 1)) == 0)
        res -= 1;

    return res;
}
