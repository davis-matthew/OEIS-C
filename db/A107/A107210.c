/*
 * A107210  Primes of the form 3x^2 + 31y^2.
 * https://oeis.org/A107210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 3x^2 + 31y^2
 * Range:     u8 n <= 9, u16 n <= 820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9749 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Ensure an array of seq_t is large enough to hold indexes [0..min_needed] */
static void ensure_seq_array(seq_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        for (size_t i = *cap; i < newcap; ++i) (*arr)[i] = 0;
        *cap = newcap;
    }
}

static inline bool qualifies(uint64_t p)
{
    if (p < 3) return false;
    idx_t y_max = isqrt(p / 31);
    for (idx_t y = 0; y <= y_max; ++y) {
        uint64_t y2 = (uint64_t)y * (uint64_t)y;
        uint64_t val31 = 31ULL * y2;
        uint64_t rem = p - val31;
        if (rem % 3 != 0) continue;
        uint64_t q = rem / 3;
        idx_t x = isqrt(q);
        if ((uint64_t)x * (uint64_t)x == q) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t size   = 0;        /* number of primes collected */
    static size_t capacity = 0;     /* allocated capacity of vals */
    static uint64_t next_candidate = 2; /* next number to test */

    while (size < (size_t)n) {
        uint64_t cand = next_candidate;
        while (!is_prime_u64(cand)) cand++;
        next_candidate = cand + 1;
        if (qualifies(cand)) {
            if (size == capacity)
                ensure_seq_array(&vals, &capacity, size + 1);
            vals[size++] = (seq_t)cand;
        }
    }
    return vals[n - 1];
}
