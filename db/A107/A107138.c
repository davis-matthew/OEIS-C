/*
 * A107138  Primes of the form 3x^2 + 11y^2.
 * https://oeis.org/A107138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as 3x^2 + 11y^2
 * Range:     u8 n <= 9, u16 n <= 837, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9841 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Grow an array of seq_t elements to at least min_cap elements. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (min_cap <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1024;
    if (new_cap < min_cap) new_cap = min_cap;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

static inline bool allowed_congruence(seq_t p)
{
    int r = (int)(p % 132);
    return r == 3 || r == 11 || r == 23 || r == 47 ||
           r == 59 || r == 71 || r == 119;
}

static inline bool is_representable(seq_t p)
{
    if (p == 3 || p == 11) return true; /* trivial representations */

    idx_t maxy = isqrt((idx_t)(p / 11));
    for (idx_t y = 0; y <= maxy; ++y) {
        seq_t rem = p - (seq_t)11 * (seq_t)y * (seq_t)y;
        if (rem % 3 != 0) continue;
        seq_t t = rem / 3;
        idx_t r = isqrt((idx_t)t);
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
        while (!is_prime_u64((uint64_t)p)) {
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
