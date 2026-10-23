/*
 * A139586  Non-Fibonacci numbers with Fibonacci number of divisors.
 * https://oeis.org/A139586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   divisor_count_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑Fibonacci numbers with a Fibonacci divisor count.
   The function a(idx_t n) returns the n-th such number (1‑based). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/floor_sqrt.h"

/* ------------------------------------------------------------------- */
/* Helper: precomputed small Fibonacci numbers (divisor counts are < 2000)
   ------------------------------------------------------------------- */
static uint32_t fib_small[32];
static size_t fib_small_len = 0;

static void init_fib_small(void)
{
    if (fib_small_len != 0)
        return;
    fib_small[0] = 1;
    fib_small[1] = 1;
    fib_small_len = 2;
    uint32_t a = 1, b = 1;
    while (b < 2000) {
        uint32_t c = a + b;
        fib_small[fib_small_len++] = c;
        a = b;
        b = c;
        if (fib_small_len >= sizeof(fib_small) / sizeof(fib_small[0]))
            break;
    }
}

static bool is_fib_small(uint32_t d)
{
    init_fib_small();
    /* binary search in a short sorted array */
    size_t lo = 0, hi = fib_small_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (fib_small[mid] == d)
            return true;
        if (fib_small[mid] < d)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* ------------------------------------------------------------------- */
/* Helper: test whether a (seq_t) is a Fibonacci number
   using the property: 5*n^2+/-4 is a perfect square
   ------------------------------------------------------------------- */
static bool is_fibonacci_big(seq_t n)
{
    seq_t n2 = n * n;
    seq_t t1 = n2 * 5 + 4;
    seq_t t2 = n2 * 5 - 4;
    seq_t r1 = floor_sqrt(t1);
    seq_t r2 = floor_sqrt(t2);
    return (r1 * r1 == t1) || (r2 * r2 == t2);
}

/* ------------------------------------------------------------------- */
/* Cache of sequence values
   ------------------------------------------------------------------- */
static seq_t *seq_cache = NULL; /* array of a(k) values */
static size_t seq_cap   = 0;    /* capacity of seq_cache */
static size_t seq_len   = 0;    /* number of values stored */

static void ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    seq_cap = new_cap;
    seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------- */
/* Search for the next qualifying number
   ------------------------------------------------------------------- */
static uint64_t candidate = 1;   /* last tested integer */

static void generate_next(void)
{
    for (;;) {
        candidate++;
        /* skip Fibonacci numbers */
        if (is_fibonacci_big((seq_t)candidate))
            continue;

        /* compute divisor count (fits in uint64 for our range) */
        uint64_t d = divisor_count_u64(candidate);
        if (is_fib_small((uint32_t)d)) {
            /* store the value */
            if (seq_len == seq_cap)
                ensure_seq_cap(seq_cap ? seq_cap * 2 : 8);
            seq_cache[seq_len++] = (seq_t)candidate;
            return;
        }
    }
}

/* ------------------------------------------------------------------- */
/* The required function
   ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 behavior is undefined */
    while ((idx_t)seq_len < n) {
        generate_next();
    }
    return seq_cache[n - 1];
}
