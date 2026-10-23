/*
 * A107210  Primes of the form 3x^2 + 31y^2.
 * https://oeis.org/A107210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 3x^2 + 31y^2
 * Range:     u8 n <= 9, u16 n <= 820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_seq_array, int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"
#include "../utils/ensure_seq_array.h"

static inline bool qualifies(uint64_t p)
{
    if (p < 3) return false;
    idx_t y_max = int_sqrt(p / 31);
    for (idx_t y = 0; y <= y_max; ++y) {
        uint64_t y2 = (uint64_t)y * (uint64_t)y;
        uint64_t val31 = 31ULL * y2;
        uint64_t rem = p - val31;
        if (rem % 3 != 0) continue;
        uint64_t q = rem / 3;
        idx_t x = int_sqrt(q);
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
        while (!is_prime(cand)) cand++;
        next_candidate = cand + 1;
        if (qualifies(cand)) {
            if (size == capacity)
                ensure_seq_array(&vals, &capacity, size + 1);
            vals[size++] = (seq_t)cand;
        }
    }
    return vals[n - 1];
}
