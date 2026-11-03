/*
 * A067831  Primes p such that sigma(p-6) < p.
 * https://oeis.org/A067831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that sigma(p-6) < p
 * Range:     u8 n <= 27, u16 n <= 1745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8771 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

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

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined offset, safety */

    idx_t count = 0;                      /* number of qualifying primes seen */
    idx_t p = 7;                          /* smallest prime > 6 */

    while (1) {
        if (is_prime(p)) {
            idx_t m = p - 6;              /* compute p-6 */
            seq_t sigma = sigma_of(m);    /* sigma(p-6) */
            if (sigma < (seq_t)p) {       /* check the inequality */
                count++;
                if (count == n) return (seq_t)p;
            }
        }
        p++;                              /* next integer to test */
    }
}
