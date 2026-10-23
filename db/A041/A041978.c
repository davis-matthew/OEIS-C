/*
 * A041978  Numerators of continued fraction convergents to sqrt(512).
 * https://oeis.org/A041978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041978
   Numerators of continued fraction convergents to sqrt(512)
   Recurrence: p_(-2)=0, p_(-1)=1,
                p_k = a_k * p_(k-1) + p_(k-2),
   where a_0=22 and for k>0 the partial quotients repeat the period
   [1,1,1,2,6,11,6,2,1,1,1,44].
   a(n) = p_n.
*/

#include <stddef.h>
#include <stdint.h>

#define PERIOD_LEN 12

static const seq_t CF_PERIOD[PERIOD_LEN] = {1,1,1,2,6,11,6,2,1,1,1,44};

seq_t a(idx_t n)
{
    seq_t p_prevprev = 0; /* p_(-2) */
    seq_t p_prev    = 1;  /* p_(-1) */
    seq_t p_cur;
    idx_t k;

    for (k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = 22;
        else
            a_k = CF_PERIOD[(k - 1) % PERIOD_LEN];
        p_cur = a_k * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_cur;
    }
    return p_prev; /* p_n */
}
