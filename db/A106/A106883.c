/*
 * A106883  Primes of the form 3x^2+3xy+4y^2, with x and y nonnegative.
 * https://oeis.org/A106883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime64, seq_cmp, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106883: primes of the form 3*x*x + 3*x*y + 4*y*y, with x,y ≥ 0 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime64.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_cmp.h"

static seq_t *prime_cache = NULL;
static idx_t   prime_count = 0;
static uint64_t current_limit = 100;
static bool     initialized   = false;

static void recompute_primes(uint64_t limit)
{
    idx_t cap = 8;
    prime_cache = seq_malloc(cap);
    prime_count = 0;

    for (uint64_t x = 0; 3ULL * x * x <= limit; ++x) {
        for (uint64_t y = 0;; ++y) {
            uint64_t val = 3ULL * x * x + 3ULL * x * y + 4ULL * y * y;
            if (val > limit) break;
            if (val <= 1) continue;
            if (is_prime64(val)) {
                if (prime_count == cap) {
                    cap <<= 1;
                    prime_cache = seq_realloc(prime_cache, cap);
                }
                prime_cache[prime_count++] = (seq_t) val;
            }
        }
    }

    if (prime_count > 1)
        qsort(prime_cache, prime_count, sizeof(seq_t), seq_cmp);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!initialized) {
        recompute_primes(current_limit);
        initialized = true;
    }

    while (prime_count < n) {
        current_limit <<= 1;
        recompute_primes(current_limit);
    }

    return prime_cache[n - 1];
}
