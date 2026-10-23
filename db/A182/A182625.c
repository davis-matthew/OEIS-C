/*
 * A182625  Numbers n for which Fibonacci(n) mod n is a Fibonacci number.
 * https://oeis.org/A182625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 2299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer m such that Fibonacci(m) mod m
   is a Fibonacci number.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_count = 0;
static idx_t next_candidate = 1;

static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure – terminate (should not happen in tests) */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

static bool is_fibonacci(seq_t r) {
    if (r == 0 || r == 1) return true;
    seq_t s1 = 5 * r * r + 4;
    seq_t t1 = floor_sqrt(s1);
    if (t1 * t1 == s1) return true;
    seq_t s2 = 5 * r * r - 4;
    seq_t t2 = floor_sqrt(s2);
    return t2 * t2 == s2;
}

static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    seq_t two_b_mod = (b * 2) % mod;
    seq_t a_mod = a % mod;
    seq_t two_b_minus_a = (two_b_mod + mod - a_mod) % mod;
    seq_t c = (a * two_b_minus_a) % mod;
    seq_t d = (a * a + b * b) % mod;
    if ((n & 1) == 0) {
        *fn = c;
        *fn1 = d;
    } else {
        *fn = d;
        *fn1 = (c + d) % mod;
    }
}

static seq_t fib_mod(idx_t n, seq_t mod) {
    seq_t f;
    seq_t zero = 0;
    fib_pair_mod(n, mod, &f, &zero);
    return f;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset guard, not used for n≥1 */

    while (cache_count < n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t r = fib_mod(next_candidate, cand);
        if (is_fibonacci(r)) {
            if (cache_count >= cache_cap) grow_cache();
            cache[cache_count++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
