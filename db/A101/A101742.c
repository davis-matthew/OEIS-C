/*
 * A101742  a(n+1) = smallest number greater than a(n) having either more
 *          divisors than a(n) or exactly as many divisors as a(n-1).
 * https://oeis.org/A101742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Not a closed form. Brute‑force search for the next term. a(1) =
 *            1. a(n+1) = smallest integer > a(n) having more divisors than
 *            a(n) or having exactly as many divisors as a(n‑1)
 * Range:     u8 n <= 37, u16 n <= 158, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Validated: standard, pass, 76 terms through n = 395 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; a(n+1) is the least k > a(n) with tau(k) > tau(a(n)) or
   tau(k) = tau(a(n-1)). The search and the divisor counts are in 64 bits
   (the terms stay small; in seq_t the search timed out under big), and the
   terms are cached. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t tau_u64(uint64_t n)
{
    uint64_t d = 1;
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            d *= e + 1;
        }
    return n > 1 ? 2 * d : d;
}

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t last = 0, tau1 = 0, tau2 = 0;     /* a(sz), tau(a(sz)), tau(a(sz-1)) */
    if (n == 0) return (seq_t)0;
    while (sz < (size_t)n) {
        uint64_t k, t;
        if (sz == 0) {
            k = 1;
            t = 1;
        } else {
            for (k = last + 1;; ++k) {
                t = tau_u64(k);
                if (t > tau1 || (sz >= 2 && t == tau2)) break;
            }
        }
        if (sz == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *v = mem_resize(seq_t, vals, cap);
            if (!v) abort();
            vals = v;
        }
        vals[sz++] = (seq_t)k;
        tau2 = tau1;
        tau1 = t;
        last = k;
    }
    return vals[n - 1];
}
