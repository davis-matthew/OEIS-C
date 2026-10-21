/*
 * A093726  Given the infinite continued fraction
 *          (1+i)+1/((1+i)+1/((1+i)+...)), where i is the square root of (-1),
 *          this is the numerator of the imaginary part of the convergents.
 * https://oeis.org/A093726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the reduced complex fraction:
   (u_{k-1} + v_{k-1} i) / w_{k-1}  →  (u_k + v_k i) / w_k
   with u_1 = 1, v_1 = 1, w_1 = 1.
   D_{k-1} = u_{k-1}^2 + v_{k-1}^2
   u_k = w_{k-1} * u_{k-1} + D_{k-1}
   v_k = D_{k-1} - w_{k-1} * v_{k-1}
   w_k = D_{k-1}
   The fraction is then reduced by gcd(u_k, v_k, w_k).
   The sequence a(n) is v_n. */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm on unsigned 128‑bit integers */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0)
    {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Initial convergent: (1+i)/1  →  u=1, v=1, w=1 */
    seq_t u = 1, v = 1, w = 1;

    if (n <= 1)
        return v;               /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute D = u*u + v*v with 128‑bit arithmetic */
        unsigned __int128 uu = (unsigned __int128)u;
        unsigned __int128 vv = (unsigned __int128)v;
        unsigned __int128 ww = (unsigned __int128)w;
        unsigned __int128 D  = uu * uu + vv * vv;

        unsigned __int128 new_u = ww * uu + D;   /* u' = w*u + D   */
        unsigned __int128 new_v = D - ww * vv;   /* v' = D - w*v   */
        unsigned __int128 new_w = D;            /* w' = D         */

        /* reduce by gcd */
        unsigned __int128 g = gcd128(new_u, new_v);
        g = gcd128(g, new_w);

        u = (seq_t)(new_u / g);
        v = (seq_t)(new_v / g);
        w = (seq_t)(new_w / g);
    }

    return v;   /* numerator of the imaginary part of the n‑th convergent */
}
