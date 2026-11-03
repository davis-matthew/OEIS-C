/*
 * A163372  a(n) = sigma(phi(sigma(n))).
 * https://oeis.org/A163372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(phi(sigma(n)))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 995 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Step 1: sigma(n) */
    seq_t s1 = sigma_of(n);

    /* Step 2: phi(sigma(n)) */
    seq_t s2 = totient_seq(s1);

    /* Step 3: sigma(phi(sigma(n))) */
    return sigma_of((idx_t)s2);
}
