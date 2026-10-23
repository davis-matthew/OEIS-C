/*
 * A042477  Denominators of continued fraction convergents to sqrt(766).
 * https://oeis.org/A042477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042477: Denominators of continued fraction convergents to sqrt(766)
   Recurrence: a(0)=1, a(1)=1.
   For n>=1, a(n)=period[(n-1)%P]*a(n-1)+a(n-2),
   where period is the periodic part of the continued fraction of sqrt(766),
   length P=44: [1,2,10,1,2,1,3,1,1,17,1,8,3,1,1,2,1,2,5,5,1,26,1,5,5,2,1,2,1,2,1,3,1,8,1,17,1,1,3,1,2,1,10,2,1,54].
*/
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static array to hold periodic coefficients of sqrt(766) */
    static uint16_t period[50];
    static size_t periodLen = 0;
    if (periodLen == 0) {
        const int N = 766;
        const int a0 = 27;   /* floor(sqrt(766)) */
        int m = 0;
        int d = 1;
        int a = a0;
        while (1) {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[periodLen++] = (uint16_t)a;
            if (a == 2 * a0)
                break;
        }
    }

    if (n == 0) {
        return (seq_t)1;
    }

    seq_t q_prev2 = 0; /* q_{-1} */
    seq_t q_prev1 = 1; /* q_{0} = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)((i - 1) % periodLen);
        uint16_t coeff = period[idx];
        seq_t q = (seq_t)coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;
}
