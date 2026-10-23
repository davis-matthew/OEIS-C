/*
 * A190683  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),3,1) and [ ]=floor.
 * https://oeis.org/A190683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n+1)*sqrt(3)) - 3*floor(n*sqrt(3)) - 1
 * Range:     u8 n <= 131, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using integer arithmetic: floor(n*sqrt(3)) = floor_sqrt(3*n^2) */
#include <stdint.h>
#include <stdbool.h>

/* integer square root for unsigned __int128 */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq > x) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 t1_n = 3 * nn * nn;
    unsigned __int128 s2 = isqrt128(t1_n);            /* floor(n*sqrt(3)) */
    unsigned __int128 n3 = 3 * nn + 1;
    unsigned __int128 t1_np = 3 * n3 * n3;
    unsigned __int128 s1 = isqrt128(t1_np);            /* floor((3n+1)*sqrt(3)) */
    unsigned __int128 val = s1 - 3 * s2 - 1;
    return (seq_t)val;
}
