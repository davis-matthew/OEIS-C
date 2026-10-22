/*
 * A004042  Periods of reciprocals of A006883, starting with first nonzero
 *          digit.
 * https://oeis.org/A004042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0 (1/2 = 0.5000..., period "0"); for n >= 2, p = (n-1)-th
 *            prime with 10 a primitive root mod p, 10^(k-1) <= p < 10^k, a(n)
 *            = 10^(k-1)*(10^(p-1)-1)/p, i.e. the p-1 digits of 10^(k-1)/p
 *            after the point (search over primes)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime, powmod_u, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/powmod_u.h"

static uint64_t *fr_list = NULL;
static idx_t fr_len = 0, fr_cap = 0;
static uint64_t fr_next = 7;

static bool full_reptend(uint64_t p) {
    uint64_t m = p - 1, q;
    if (p == 2 || p == 5 || !is_prime(p)) return false;
    for (q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            if (powmod_u(10, (p - 1) / q, p) == 1) return false;
            while (m % q == 0) m /= q;
        }
    }
    if (m > 1 && powmod_u(10, (p - 1) / m, p) == 1) return false;
    return true;
}

static uint64_t get_fr(idx_t i) {
    while (fr_len <= i) {
        if (full_reptend(fr_next)) {
            if (fr_len == fr_cap) {
                fr_cap = fr_cap ? fr_cap * 2 : 1024;
                fr_list = (uint64_t *)realloc(fr_list, (size_t)fr_cap * sizeof(uint64_t));
            }
            fr_list[fr_len++] = fr_next;
        }
        fr_next += 2;
    }
    return fr_list[i];
}

seq_t a(idx_t n) {
    uint64_t p, r, cnt, i, chunk, cpow;
    seq_t result = 0;
    bool fixed = seq_is_fixed_width();
    if (n <= 1) return 0;
    p = get_fr(n - 2);
    r = 1;
    while (r * 10 <= p) r *= 10;
    cnt = p - 1;
    chunk = 0;
    cpow = 1;
    for (i = 0; i < cnt; i++) {
        uint64_t d = (10 * r) / p;
        r = (10 * r) % p;
        if (fixed) {
            result = result * 10 + (seq_t)d;
        } else {
            chunk = chunk * 10 + d;
            cpow *= 10;
            if (cpow == 1000000000ULL) {
                result = result * (seq_t)cpow + (seq_t)chunk;
                chunk = 0;
                cpow = 1;
            }
        }
    }
    if (!fixed && cpow > 1)
        result = result * (seq_t)cpow + (seq_t)chunk;
    return result;
}
