/*
 * A077082  Largest value arising when phi(sigma(n)+phi(n))=A077080 is
 *          iterated with initial value of n.
 * https://oeis.org/A077082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max[FixedPointList[A077080, n]]
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         where A077080(n) = phi( sigma(n) + phi(n) )   */
/*         Sequence:  a(n) = largest value encountered
 *         while iterating A077080 starting from n until a fixed point (A077080(x)=x). */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;
    seq_t best    = current;

    while (1) {
        /* Compute the next value of A077080. */
        seq_t phi_n   = calc_phi(current);          /* phi(current)          */
        seq_t sigma_n = sigma_of((idx_t)current);   /* sigma(current)        */
        seq_t sum     = sigma_n + phi_n;            /* sigma+phi             */
        seq_t next    = calc_phi(sum);              /* phi(sigma+phi)        */

        if (next == current)
            break;                                 /* fixed point reached   */

        if (next > best)
            best = next;                           /* update maximum       */

        current = next;
    }

    return best;
}
