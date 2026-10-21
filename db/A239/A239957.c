/*
 * A239957  a(n) = |{0 < g < prime(n): g is a primitive root modulo prime(n)
 *          of the form k^2 + 1}|.
 * https://oeis.org/A239957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive roots g modulo prime(n) of the form
 *            k^2 + 1
 * Range:     u8 n <= 18, u16 n <= 1562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

/* Count the primitive roots of the form k^2+1 modulo the n-th prime. */
static bool is_primitive_root(seq_t g, seq_t p,
                              const uint32_t *factors, size_t fcnt)
{
    /* For each prime divisor q of p-1, check g^((p-1)/q) mod p != 1. */
    uint64_t m = (uint64_t)(p - 1);
    for (size_t i = 0; i < fcnt; ++i) {
        uint32_t q = factors[i];
        uint64_t exp = m / q;
        if (mod_pow(g, exp, p) == 1)
            return false;
    }
    return true;
}

/* Factor the 64‑bit integer n and store the distinct prime factors in
   the array 'factors' (maximum 20 elements). The number of distinct
   factors is stored in '*fcnt'. */
static void factor_uint64(uint64_t n, uint32_t *factors, size_t *fcnt)
{
    *fcnt = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            factors[(*fcnt)++] = (uint32_t)d;
            while (n % d == 0)
                n /= d;
        }
    }
    if (n > 1)
        factors[(*fcnt)++] = (uint32_t)n;
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-based). */
    seq_t p = (seq_t)get_prime(n);      /* p is uint32_t, cast to seq_t */
    if (p <= 2) {                       /* primes 2 and 3 are handled quickly */
        /* Only g = 1 works for p = 2, g = 2 works for p = 3 */
        return (seq_t)(n == 1 ? 1 : (n == 2 ? 1 : 0));
    }

    uint32_t factors[20];
    size_t fcnt;
    uint64_t pm1 = (uint64_t)(p - 1);
    factor_uint64(pm1, factors, &fcnt);

    /* Count suitable k. */
    seq_t maxk_seq = floor_sqrt(p - 1);
    idx_t maxk = (idx_t)maxk_seq;

    seq_t count = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;
        seq_t g = kk + 1;
        if (g >= p)
            continue;
        if (is_primitive_root(g, p, factors, fcnt))
            ++count;
    }
    return count;
}
