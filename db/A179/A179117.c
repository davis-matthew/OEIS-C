/*
 * A179117  a(n) = Sum_{d|n} phi(d^phi(d)).
 * https://oeis.org/A179117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d^phi(d))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   calc_phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/calc_phi.h"

/* Compute the n-th term of sequence A179117.  The function may be
   called in any order; it is thread‑safe for the single–threaded
   evaluation environment used by the OEIS test harness. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1 */
    uint64_t un = (uint64_t)n;           /* use native unsigned for loops */
    seq_t sum = 0;

    for (uint64_t d = 1; d * d <= un; ++d) {
        if (un % d == 0) {
            /* ----- divisor d ----- */
            seq_t phi_d = calc_phi((seq_t)d);
            idx_t   exp_d = (idx_t)(phi_d - 1);  /* phi_d fits in 64‑bit */
            seq_t   pow_d = seq_pow((seq_t)d, exp_d);
            seq_t   term_d = phi_d * pow_d;      /* phi(d) * d^(phi(d)-1) */
            sum += term_d;

            /* ----- paired divisor n/d (if distinct) ----- */
            uint64_t d2 = un / d;
            if (d2 != d) {
                seq_t phi_d2 = calc_phi((seq_t)d2);
                idx_t   exp_d2 = (idx_t)(phi_d2 - 1);
                seq_t   pow_d2 = seq_pow((seq_t)d2, exp_d2);
                seq_t   term_d2 = phi_d2 * pow_d2;
                sum += term_d2;
            }
        }
    }
    return sum;
}
