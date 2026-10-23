/*
 * A135061  a(n) = minimum (floor(n^3/m) + m) for any integer m >= 1.
 * https://oeis.org/A135061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2 * n^(3/2)) = floor( sqrt(4 * n^3) )
 * Range:     u8 n <= 25, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 64;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* value not defined for n<1, but safe */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 value = 4 * nn * nn * nn;   /* 4 * n^3 */
    unsigned __int128 root  = isqrt128(value);    /* floor(sqrt(4*n^3)) == a(n) */
    return (seq_t)root;
}
