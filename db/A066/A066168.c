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
 * Helpers:   next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/next_prime.h"

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
