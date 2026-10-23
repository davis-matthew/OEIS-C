/*
 * A105903  Primes for which -31 is a primitive root.
 * https://oeis.org/A105903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, pow_mod, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that -31 is a primitive root modulo p */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/pow_mod.h"
#include "../utils/push_seq.h"

static bool negative31_is_primitive_root(uint64_t p)
{
    if (p == 2) return true;
    if (p == 31) return false;

    /* compute (-31) mod p */
    uint64_t base = (p - (31 % p)) % p;

    /* factor p-1 to get its distinct prime divisors */
    uint64_t m = p - 1;
    uint64_t pf[64];
    size_t pf_cnt = 0;

    if (m % 2 == 0) {
        pf[pf_cnt++] = 2;
        while (m % 2 == 0) m /= 2;
    }
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            pf[pf_cnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) pf[pf_cnt++] = m;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t q = pf[i];
        if (pow_mod(base, (p - 1) / q, p) == 1)
            return false;   /* order divides (p-1)/q, not primitive */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* memoized list of primes satisfying the condition */
    static seq_t *found = NULL;
    static idx_t found_cap = 0;
    static idx_t found_cnt = 0;
    static uint64_t next_candidate = 2;

    while (found_cnt < n) {
        if (is_prime(next_candidate) && negative31_is_primitive_root(next_candidate)) {
            push_seq(&found, &found_cap, &found_cnt, (seq_t)next_candidate);
        }
        /* advance to next odd candidate after handling 2 */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
    return found[n - 1];
}
