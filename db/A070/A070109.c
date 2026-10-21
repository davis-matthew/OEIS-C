/*
 * A070109  Number of right integer triangles with perimeter n and relatively
 *          prime side lengths.
 * https://oeis.org/A070109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive Pythagorean triples with perimeter n
 *            (i.e. a right integer triangle of coprime sides)
 * Range:     u8 n <= 9250, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Only even perimeters can occur */
    if (n & 1ULL)
        return 0;

    /* t = n/2  -->  we need solutions to m(m+n) = t with m > n > 0, gcd(m,n)=1,
       and m,n of opposite parity. */
    idx_t t  = n >> 1;          /* n / 2 */
    seq_t cnt = 0;

    for (idx_t m = 1; (__int128)m * m <= t; ++m) {
        if (t % m != 0)
            continue;

        idx_t n1 = t / m - m;   /* n1 = n */
        if (n1 == 0)
            continue;           /* n must be positive */

        if (m <= n1)            /* need m > n */
            continue;

        /* gcd(m,n1) must be 1 */
        uint64_t a = m;
        uint64_t b = n1;
        while (b != 0) {
            uint64_t tmp = a % b;
            a = b;
            b = tmp;
        }
        if (a != 1)
            continue;

        /* m and n must be of opposite parity  */
        if ((m & 1ULL) == (n1 & 1ULL))
            continue;

        ++cnt;
    }
    return cnt;
}
