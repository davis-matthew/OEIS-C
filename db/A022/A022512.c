/*
 * A022512  Describe previous term from the right (method A - initial term is
 *          8).
 * https://oeis.org/A022512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 8
   a(n) = next_term(a(n-1))  for n > 0
   where next_term(x) obtains the next term by reading x's digits
   from right to left, grouping consecutive identical digits,
   and writing for each group: <count><digit> (count in decimal). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Append one run (count followed by digit) to the current number. */
static seq_t append_run(seq_t cur, seq_t count, seq_t digit)
{
    /* number of decimal digits of count */
    idx_t k = (idx_t)num_digits(count) + 1;      /* +1 for the digit itself   */
    seq_t mult = pow10(k);                     /* 10^k                       */
    seq_t part = mul_small(count, 10) + digit; /* count*10 + digit           */
    return cur * mult + part;
}

/* Construct the next term of the sequence from the current term x. */
static seq_t next_term(seq_t x)
{
    seq_t result = 0;
    seq_t last_digit = 0;
    seq_t run = 0;
    bool first = true;

    while (x > 0) {
        seq_t d = x % 10;
        x = x / 10;
        if (first) {
            last_digit = d;
            run = 1;
            first = false;
        } else {
            if (d == last_digit) {
                run++;                       /* same digit – extend run   */
            } else {
                result = append_run(result, run, last_digit);
                last_digit = d;
                run = 1;
            }
        }
    }
    if (!first) {
        result = append_run(result, run, last_digit);
    }
    return result;
}

static seq_t *cache = NULL;     /* array of previously computed terms   */
static idx_t  cache_len = 0;    /* elements currently stored            */
static idx_t  cache_cap = 0;    /* allocated capacity                   */

seq_t a(idx_t n)
{
    /* initialize cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 8;
        cache_len = 1;
    }

    /* extend cache up to index n if needed */
    if (n >= cache_len) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* grow geometrically */
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = next_term(cache[i-1]);
        }
        cache_len = n + 1;
    }
    return cache[n];
}
