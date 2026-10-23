/*
 * A184931  a(n) = n + [rn/u] + [sn/u] + [tn/u], where []=floor and
 *          r=sin(Pi/2), s=sin(Pi/3), t=sin(Pi/4), u=sin(Pi/5).
 * https://oeis.org/A184931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [rn/u] + [sn/u] + [tn/u] where r = sin(π/2), s =
 *            sin(π/3), t = sin(π/4), u = sin(π/5)
 * Range:     u8 n <= 47, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the necessary constants once */
    static const long double PI  = acosl(-1.0L);
    static const long double u   = sinl(PI / 5.0L);
    static const long double r   = sinl(PI / 2.0L);   /* = 1 */
    static const long double s   = sinl(PI / 3.0L);
    static const long double t   = sinl(PI / 4.0L);

    static const long double r_over_u = r / u;
    static const long double s_over_u = s / u;
    static const long double t_over_u = t / u;

    /* Compute each floor term using 64‑bit integers,
       then cast to seq_t for the final accumulation. */
    uint64_t f1 = (uint64_t)floorl((long double)n * r_over_u);
    uint64_t f2 = (uint64_t)floorl((long double)n * s_over_u);
    uint64_t f3 = (uint64_t)floorl((long double)n * t_over_u);

    seq_t result = (seq_t)n;  /* n as seq_t */
    result += (seq_t)f1;
    result += (seq_t)f2;
    result += (seq_t)f3;

    return result;
}
