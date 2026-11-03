/*
 * A062041  Positive numbers whose product of digits is 9 times their sum.
 * https://oeis.org/A062041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 299, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 482 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th positive integer whose product of
   decimal digits equals nine times their sum, found by enumerating
   integers in increasing order and keeping those that satisfy the
   digit‑product ≶ 9×digit‑sum condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/mem.h"

/* Test whether x satisfies the required property: product of digits
   equals 9 times the sum of digits. */
static int good_seq_t(seq_t x)
{
    if (x == 0) return 0;             /* positive integers only */

    seq_t prod = 1;
    seq_t sum = 0;
    seq_t temp = x;

    while (temp > 0) {
        seq_t d = temp % 10;          /* last decimal digit */
        sum += d;
        prod *= d;
        temp = temp / 10;
    }

    return prod == (seq_t)9 * sum;
}

static seq_t *cache = NULL;         /* cached terms of the sequence   */
static size_t cache_len = 0;        /* number of terms stored         */
static size_t cache_cap = 0;        /* allocated capacity of the cache */
static seq_t last_checked = 0;      /* last integer tested            */

/* Append a value to the cache, resizing it if necessary. */
static void push_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();              /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the nth term of A062041 (1‑based). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cur = last_checked;
        ++cur;                         /* next integer to test */
        if (good_seq_t(cur)) {
            push_cache(cur);
        }
        last_checked = cur;
    }
    return cache[(size_t)n - 1];
}
