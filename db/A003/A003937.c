/*
 * A003937  Order of universal Chevalley group B_8(q), q = prime power.
 * https://oeis.org/A003937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^64 * Product_{i=1..8} (q^(2i) - 1), where q =
 *            A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   is_prime_power_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_power_u.h"

static uint64_t *pp_cache = NULL;
static idx_t pp_count = 0;
static idx_t pp_cap = 0;
static uint64_t pp_next = 2;

static void ensure_pp(idx_t needed) {
    while (pp_count < needed) {
        if (is_prime_power_u(pp_next)) {
            if (pp_count >= pp_cap) {
                pp_cap = pp_cap == 0 ? 64 : pp_cap * 2;
                pp_cache = (uint64_t *)realloc(pp_cache, pp_cap * sizeof(uint64_t));
            }
            pp_cache[pp_count++] = pp_next;
        }
        pp_next++;
    }
}

static seq_t spow(seq_t b, unsigned e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1u) r *= b;
        e >>= 1;
        if (e > 0) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    ensure_pp(n);
    seq_t q = (seq_t)pp_cache[n - 1];
    seq_t q2 = q * q;
    seq_t result = spow(q, 64);
    seq_t p = 1;
    for (int i = 1; i <= 8; i++) {
        p *= q2;
        result *= p - 1;
    }
    return result;
}
