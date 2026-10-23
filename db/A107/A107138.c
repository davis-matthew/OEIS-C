/*
 * A107138  Primes of the form 3x^2 + 11y^2.
 * https://oeis.org/A107138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as 3x^2 + 11y^2
 * Range:     u8 n <= 9, u16 n <= 837, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allowed_congruence, ensure_capacity, integer_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/integer_sqrt.h"
#include "../utils/ensure_capacity.h"
#include "../utils/allowed_congruence.h"

static inline bool is_representable(seq_t p)
{
    if (p == 3 || p == 11) return true; /* trivial representations */

    idx_t maxy = integer_sqrt((idx_t)(p / 11));
    for (idx_t y = 0; y <= maxy; ++y) {
        seq_t rem = p - (seq_t)11 * (seq_t)y * (seq_t)y;
        if (rem % 3 != 0) continue;
        seq_t t = rem / 3;
        idx_t r = integer_sqrt((idx_t)t);
        if ((seq_t)r * r == t) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n=0, but safe fallback */

    /* static cache of found terms */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_size = 0;
    static seq_t next_candidate = 3; /* start searching from 3 */

    while (seq_size < n) {
        /* find the next prime */
        seq_t p = next_candidate;
        while (!is_prime((uint64_t)p)) {
            p += 2; /* skip even numbers */
        }

        /* test whether p fits the quadratic form */
        if (allowed_congruence(p) && is_representable(p)) {
            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                ensure_capacity(new_cap, &seq, &seq_cap);
            }
            seq[seq_size++] = p;
        }

        /* advance to the next odd number after the found prime */
        next_candidate = p + 2;
    }

    return seq[n - 1];
}
