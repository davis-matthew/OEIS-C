/*
 * A098394  Next larger integer including the smallest digit of n.
 * https://oeis.org/A098394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9018 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1
 *   for n > 1, let d = smallest decimal digit of a(n‑1);
 *   a(n) = the smallest integer m > a(n‑1) that contains digit d
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* return true if x contains the digit d (0–9) */
static bool contains_digit(seq_t x, int d)
{
    if (x == 0)
        return d == 0;
    unsigned long long target = (unsigned long long)d;
    while (x > 0) {
        unsigned long long digit = (unsigned long long)(x % 10);
        if (digit == target)
            return true;
        x /= 10;
    }
    return false;
}

/* return the smallest decimal digit of x (0–9) */
static int min_digit(seq_t x)
{
    if (x == 0)
        return 0;
    int min = 9;
    while (x > 0) {
        /* digit = x % 10; the result is a seq_t that can be cast to a small integer */
        unsigned long long d = (unsigned long long)(x % 10);
        if (d < (unsigned long long)min)
            min = (int)d;
        if (min == 0)
            break;
        x /= 10;
    }
    return min;
}

seq_t a(idx_t n)
{
    /* cache the terms that have already been computed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of computed terms in cache */

    if (n == 0)                     /* the sequence is 1‑based; return 0 for 0 */
        return 0;

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) { /* allocation failed */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {          /* correspond to a(1) */
            cache[i] = 1;
            continue;
        }
        seq_t prev = cache[i - 1];          /* a(i) depends on a(i‑1) */
        int d = min_digit(prev);             /* smallest digit of previous term */
        seq_t m = prev + 1;                  /* start searching from next integer */
        while (!contains_digit(m, d))
            ++m;                             /* find the first with digit d */
        cache[i] = m;
    }

    cache_len = n;
    return cache[n - 1];
}
