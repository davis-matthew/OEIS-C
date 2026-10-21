/*
 * A070104  Number of integer triangles with perimeter n and relatively prime
 *          side lengths which are obtuse and scalene.
 * https://oeis.org/A070104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of integer triangles with perimeter n that are
 *            scalene, obtuse and have relatively prime side lengths (i.e.
 *            gcd(a,b,c)=1)
 * Range:     u8 n <= 136, u16 n <= 2152, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used in gcd64 */

static inline uint64_t gcd64(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t count = 0;

    /* a < b < c, a+b+c = n, triangle inequalities,
       must satisfy a + b > c  <=>  a + b > n/2   */
    idx_t half = n / 2;          /* floor(n/2) */

    /* a ranges from 1 up to floor(n/3) so that a <= b <= c */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* lower bound on b to ensure a + b > n/2 and distinctness a < b */
        idx_t b_min = half + 1 - a;          /* from a + b > n/2  ->  b > n/2 - a */
        if (b_min < a + 1) b_min = a + 1;    /* enforce b > a */
        if (b_min > (n - a - 1) / 2) continue;  /* no valid b for this a */

        /* upper bound on b so that c = n-a-b > b (scalene and order) */
        idx_t b_max = (n - a - 1) / 2;        /* from c > b  ->  b <= (n-a-1)/2 */

        for (idx_t b = b_min; b <= b_max; ++b) {
            idx_t c = n - a - b;             /* automatically > b */

            /* obtuse condition: largest side c satisfies c^2 > a^2 + b^2 */
            idx_t sqc = (idx_t)c * c;
            idx_t sqab = (idx_t)a * a + (idx_t)b * b;
            if (sqc <= sqab) continue;

            /* primitiveness: gcd(a,b,c) == 1 */
            if (gcd64(a, gcd64(b, c)) == 1) {
                count = count + 1;           /* safe addition for seq_t */
            }
        }
    }

    return count;
}
