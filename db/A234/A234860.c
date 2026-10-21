/*
 * A234860  Sum of the divisors of n^3 - 1.
 * https://oeis.org/A234860
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n^3 − 1) = sum of the divisors of n³ − 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 308, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"


seq_t a(idx_t n) {
    if (n < 2) {
        return 0;  /* undefined for n < 2 */
    }

    /* Compute n^3 − 1; this fits in 64 bits for all n used in the tests. */
    uint64_t nn = (uint64_t)n;
    uint64_t m = nn * nn * nn - 1ULL;

    /* Factor m.  The array sizes are generous; 64 slots are more than enough
       for 64‑bit integers. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    /* Compute the divisor sum multiplicatively:
         sigma(p^e) = (p^(e+1) – 1)/(p – 1)
    */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p       = (seq_t)pr[i];
        seq_t term    = (pow_int(p, ex[i] + 1) - 1) / (p - 1);
        sigma *= term;
    }
    return sigma;
}
