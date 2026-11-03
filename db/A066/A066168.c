/*
 * A066168  a(n) = least k such that phi(k) > sigma(n).
 * https://oeis.org/A066168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = least prime p such that φ(p) > σ(n), which is the
 *            smallest prime > σ(n) + 1
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 961 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

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
    /* Compute σ(n). */
    seq_t sigma_val = sigma_of(n);

    /* σ(n) + 1 fits easily in 64‑bit for n ≤ 10000. */
    seq_t sigma_plus1 = sigma_val + (seq_t)1;
    uint64_t start = (uint64_t)sigma_plus1;  /* start = σ(n)+1 */

    /* Find the smallest prime > start. */
    uint64_t prime = next_prime(start);

    return (seq_t)prime;
}
