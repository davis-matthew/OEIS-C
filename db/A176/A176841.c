/*
 * A176841  a(n) is the number of iterations of f(n) = n-phi(tau(n)) needed to
 *          reach 1.
 * https://oeis.org/A176841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=1+a(n-phi(tau(n))) where tau(n) is the
 *            number of divisors of n
 * Range:     u8 n <= 1136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses the provided helpers 
 * divisor_count_u64(n) to get tau(n) and calc_phi(m) to get phi(m). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* a(1) is 0 by definition */
    if (n <= 1) {
        return 0;
    }

    idx_t cur = n;
    seq_t steps = 0;

    while (cur > 1) {
        /* Compute tau(cur) */
        uint64_t tau = divisor_count_u64(cur);

        /* Compute phi(tau) */
        seq_t phi_val = calc_phi((seq_t)tau);

        /* Move to the next value.  phi_val <= cur-1 for cur>1. */
        cur -= (idx_t)phi_val;
        steps++;
    }

    return steps;
}
