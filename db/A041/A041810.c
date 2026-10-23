/*
 * A041810  Numerators of continued fraction convergents to sqrt(426).
 * https://oeis.org/A041810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041810: Numerators of continued fraction convergents to sqrt(426)
   Recurrence: a(0)=20, a(1)=21
                a(n)=b(n)*a(n-1)+a(n-2)  for n≥2
   where              b(1)=1,b(2)=1,b(3)=1,b(4)=3,b(5)=2,b(6)=6,
   and thereafter the period 12 list
       {1,1,1,3,2,6,2,3,1,1,1,40} repeats. */

#include <stddef.h> /* needed for size_t in static arrays */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)20u;          /* a(0) = 20 */
    if (n == 1u) return (seq_t)21u;          /* a(1) = 21 */

    /* Partial quotients after the first term (period 12). */
    static const unsigned short period[12] = {
        1u, 1u, 1u, 3u, 2u, 6u,
        2u, 3u, 1u, 1u, 1u, 40u
    };

    /* Initialize the recurrence: p0 = 20, p1 = 21. */
    seq_t p_prev2 = (seq_t)20u; /* p0  */
    seq_t p_prev1 = (seq_t)21u; /* p1  */

    for (idx_t i = 2u; i <= n; ++i) {
        seq_t bi = (seq_t)period[(unsigned int)((i - 1u) % 12u)];
        seq_t p_curr = bi * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_prev1;
}
