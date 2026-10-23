/*
 * A138224  a(n) is the divisor of n that is nearest to tau(n), the number of
 *          divisors of n. In case of a tie, take the smaller of those two
 *          divisors.
 * https://oeis.org/A138224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7764, u16 n <= 7764, u32 n <= 7764, u64 n <= 7764,
 *            u128 n <= 7764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Find the divisor of n that is closest to τ(n), the number of divisors
   of n.  If two divisors are equally close, take the smaller one.
   This implements the definition of OEIS sequence A138224.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute τ(n) (number of divisors of n) */
    seq_t tau = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i == n / i)          /* perfect square – count once */
                ++tau;
            else
                tau += 2;             /* count both i and n/i */
        }
    }

    /* find the divisor closest to tau; tie → smaller divisor   */
    seq_t bestdist = tau;      /* worst-case initial distance */
    seq_t bestdiv  = 0;        /* will be overwritten on first divisor */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            seq_t d1 = (seq_t)i;
            seq_t diff1 = (d1 > tau) ? (d1 - tau) : (tau - d1);
            if (diff1 < bestdist || (diff1 == bestdist && d1 < bestdiv)) {
                bestdist = diff1;
                bestdiv  = d1;
            }

            /* paired divisor n / i, if distinct */
            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                seq_t diff2 = (d2 > tau) ? (d2 - tau) : (tau - d2);
                if (diff2 < bestdist || (diff2 == bestdist && d2 < bestdiv)) {
                    bestdist = diff2;
                    bestdiv  = d2;
                }
            }
        }
    }

    return bestdiv;
}
