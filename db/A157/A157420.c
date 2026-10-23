/*
 * A157420  a(0)=1. a(n) = the smallest integer coprime to a(n-1) and greater
 *          than the n-th Fibonacci number.
 * https://oeis.org/A157420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(0)=1; for n≥1, a(n)=the smallest integer m>F_n such that *
 *            gcd(a(n-1),m)=1 (F_n is the n-th Fibonacci number, F_0=0,F_1=1)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *fib_cache = NULL;
static size_t fib_cap   = 0;

static seq_t *a_cache   = NULL;
static size_t   a_cap   = 0;
static size_t   a_len   = 0;   /* number of computed terms (a_len‑1 is the highest stored index) */

/* Ensure fib_cache contains Fibonacci numbers up to index n (inclusive). */
static void ensure_fib(idx_t n)
{
    if (fib_cache == NULL) {
        fib_cap = 2;
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) exit(1);
        fib_cache[0] = 0;
        fib_cache[1] = 1;
    }
    if (fib_cap <= (size_t)n) {
        size_t new_cap = fib_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = fib_cap; i < new_cap; ++i)
            tmp[i] = tmp[i-1] + tmp[i-2];
        fib_cache = tmp;
        fib_cap   = new_cap;
    }
}

/* Ensure a_cache has capacity for indices up to n and guarantees that a(0)=1 is stored. */
static void ensure_a_capacity(idx_t n)
{
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) exit(1);
        a_cache[0] = 1;   /* a(0) */
        a_len   = 1;
    }
    if (a_cap <= (size_t)n) {
        size_t new_cap = a_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    ensure_a_capacity(n);
    ensure_fib(n);

    /* Compute any missing terms up to n */
    for (size_t idx = a_len; idx <= (size_t)n; ++idx) {
        seq_t threshold = fib_cache[idx];   /* F_idx */
        seq_t candidate = threshold + 1;     /* smallest integer > F_idx */

        seq_t prev = a_cache[idx-1];
        while (gcd(prev, candidate) != 1)
            ++candidate;

        a_cache[idx] = candidate;
    }
    a_len = (size_t)n + 1;

    return a_cache[(size_t)n];
}
