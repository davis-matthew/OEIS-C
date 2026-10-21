/*
 * A062402  a(n) = sigma(phi(n)).
 * https://oeis.org/A062402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(phi(n))
 * Range:     u8 n <= 46, u16 n <= 4506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, phi_uint64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of Euler's totient of n */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi_uint64.h"
#include "../utils/factor_uint64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* compute φ(n) */
    uint64_t phi_n = phi_uint64((uint64_t)n);
    if (phi_n == 0) return (seq_t)0;          /* safety: n=0 never called */

    /* special case φ(n)=1 → σ(1)=1 */
    if (phi_n == 1) return (seq_t)1;

    /* factor φ(n) */
    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(phi_n, fac, exp, &cnt);

    /* compute σ(φ(n)) via prime‑power product */
    seq_t result = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        /* term = (p^(e+1) – 1) / (p – 1) */
        seq_t p = (seq_t)fac[i];
        seq_t term = (pow_uint((uint64_t)fac[i], (idx_t)(exp[i] + 1)) - (seq_t)1) / (p - (seq_t)1);
        result *= term;
    }
    return result;
}
