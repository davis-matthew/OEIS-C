/*
 * A055741  Phi(n) has more distinct prime factors than n.
 * https://oeis.org/A055741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k (k ≥ 1) such that the number of
 *            distinct prime factors of φ(k) is strictly larger than the
 *            number of distinct prime factors of k. (OEIS A055741)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega_u64.h"

/* Euler's totient φ(n) for unsigned 64‑bit n */
static uint64_t phi_u64(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t result = n;
    if (n % 2ULL == 0) { result -= result / 2ULL; while (n % 2ULL == 0) n /= 2ULL; }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) { result -= result / p; while (n % p == 0) n /= p; }
    }
    if (n > 1) result -= result / n;
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t found = 0;
    uint64_t k = 2;
    while (found < n) {
        uint64_t phi = phi_u64(k);
        unsigned int omega_k = omega_u64(k);
        unsigned int omega_phi = omega_u64(phi);
        if (omega_phi > omega_k) {
            found++;
            if (found == n) {
                return (seq_t)k;
            }
        }
        k++;
        /* The sequence grows roughly linearly, so this loop is fast for n ≤ 10000 */
    }
    return 0;  /* never reached */
}
