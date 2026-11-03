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
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         where A077080(n) = phi( sigma(n) + phi(n) )   */
/*         Sequence:  a(n) = largest value encountered
 *         while iterating A077080 starting from n until a fixed point (A077080(x)=x). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;
    seq_t best    = current;

    while (1) {
        /* Compute the next value of A077080. */
        seq_t phi_n   = totient_seq(current);          /* phi(current)          */
        seq_t sigma_n = sigma_of((idx_t)current);   /* sigma(current)        */
        seq_t sum     = sigma_n + phi_n;            /* sigma+phi             */
        seq_t next    = totient_seq(sum);              /* phi(sigma+phi)        */

        if (next == current)
            break;                                 /* fixed point reached   */

        if (next > best)
            best = next;                           /* update maximum       */

        current = next;
    }

    return best;
}
