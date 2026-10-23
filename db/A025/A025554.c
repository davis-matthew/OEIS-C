/*
 * A025554  a(n) = (Sum_{i=0..n-1} (-1)^i/b(i)) * LCM{b(i): i=0..n-1}, where
 *          b(i) = C(i,floor(i/2)).
 * https://oeis.org/A025554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{i=0}^{n-1} (-1)^i / C(i, floor(i/2))) * * lcm{C(i,
 *            floor(i/2)) : 0 <= i < n}
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, but guard anyway */

    seq_t L = 1;      /* current lcm */
    seq_t sum = 1;    /* current a(k) for k = 1 */
    seq_t b = 1;      /* b(0) = C(0,0) */

    for (idx_t i = 1; i < n; ++i) {
        /* compute b(i) from b(i-1) */
        if (i & 1u) {                       /* i is odd: i = 2m+1 */
            seq_t denom = (seq_t)((i + 1) >> 1);   /* (i+1)/2 = m+1 */
            b = b * (seq_t)i / denom;
        } else {                            /* i is even: i = 2m   */
            seq_t denom = (seq_t)(i >> 1);         /* i/2 = m     */
            b = b * (seq_t)i / denom;
        }

        seq_t oldL = L;
        seq_t g = gcd(oldL, b);
        seq_t Q = b / g;                   /* factor to multiply L */
        seq_t oldL_div_g = oldL / g;       /* L / gcd(L,b) */

        /* update sum: newSum = Q * oldSum + (-1)^i * (L / g)   */
        if (i & 1u)                         /* odd i   */
            sum = Q * sum - oldL_div_g;
        else
            sum = Q * sum + oldL_div_g;

        L = oldL * Q;  /* updated lcm for next iteration */
    }

    return sum;
}
