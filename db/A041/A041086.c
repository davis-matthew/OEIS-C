/*
 * A041086  Numerators of continued fraction convergents to sqrt(51).
 * https://oeis.org/A041086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for A041086: numerators of continued fraction convergents
 *  to sqrt(51).  The continued fraction of sqrt(51) is
 *  sqrt(51) = [7; 7, 14, 7, 14, …].  Denote the numerators by p_n
 *  with the initial conditions p_{-2}=0, p_{-1}=1.  Then
 *
 *      p_n = a_n * p_{n-1} + p_{n-2},
 *
 *  where a_0 = 7 and for n ≥ 1,
 *  a_n = 7 if n is odd, a_n = 14 if n is even.
 *
 *  In particular
 *      a(0) = 7,
 *      a(1) = 50,
 *      a(n) = ((n%2==0) ? 14 : 7) * a(n-1) + a(n-2)   (n ≥ 2)
 *
 *  The code below implements this recurrence efficiently for any
 *  seq_t (unbounded or bounded integer type) and any idx_t.
 */

#include <stdint.h>   /* for uint64_t, but idx_t is provided by the build */

seq_t a(idx_t n)
{
    /* p_{-2}=0, p_{-1}=1  */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t result  = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* Determine the partial quotient a_i.  For i==0, a_0 = 7.
           For i ≥ 1, a_i = 7 when i is odd, 14 when i is even.   */
        seq_t ai = (i == 0) ? (seq_t)7
                            : ((i & 1) ? (seq_t)7 : (seq_t)14);

        /* p_i = a_i * p_{i-1} + p_{i-2}  */
        seq_t p = ai * p_prev1 + p_prev2;

        if (i == n)
            result = p;

        /* Shift previous two values for next iteration.  */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return result;
}
