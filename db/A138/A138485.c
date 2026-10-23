/*
 * A138485  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 1.
 * https://oeis.org/A138485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0) = 1
 * a(n+1) is obtained from a(n) by:
 *   * Count how many times each digit 0–9 appears in a(n).
 *   * Scan a(n) from right (least‑significant digit) to left.
 *   * For each digit encountered for the first time, output
 *     the count of that digit (in decimal) followed by the digit.
 *
 * Example: a(5)=211312 -> 223113
 *   counts: 0→0, 1→3, 2→2, 3→1, others→0
 *   scan right‑to‑left: 2,1,3,1,1,2
 *   first occurrences: 2,1,3 -> output 2 2, 3 1, 1 3 -> 223113
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* External linkage required by the harness */
seq_t a(idx_t n);

/* Forward declarations of helper functions */
static seq_t compute_next(seq_t cur);
static void ensure_cache(idx_t n);

/* --------------------------------------------------------------------- */
/*          Static cache for memoisation                                */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* --------------------------------------------------------------------- */
/*  Return the n-th term of the sequence                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

/* --------------------------------------------------------------------- */
/*  Compute a(n+1) from a(n)                                           */
/* --------------------------------------------------------------------- */
static seq_t compute_next(seq_t cur)
{
    /* Count digits and order of first appearance from right. */
    unsigned int cnt[10] = {0};
    bool seen[10] = {false};

    /* ----- First pass: count occurrences ----- */
    if (cur == (seq_t)0) {
        cnt[0] = 1;
    } else {
        seq_t temp = cur;
        while (temp > (seq_t)0) {
            unsigned int d = (unsigned int)(temp % (seq_t)10);
            cnt[d]++;
            temp /= (seq_t)10;
        }
    }

    /* ----- Second pass: output in required order ----- */
    seq_t result = (seq_t)0;

    if (cur == (seq_t)0) {
        /* Special case: only digit 0 */
        unsigned int d = 0;
        unsigned int c = cnt[0];
        /* Emit count c */
        if (c > 0) {
            /* Convert c to decimal digits (most significant first) */
            unsigned int digits[32];
            size_t sz = 0;
            unsigned int temp_c = c;
            while (temp_c > 0) {
                digits[sz++] = temp_c % 10;
                temp_c /= 10;
            }
            for (size_t i = sz; i > 0; --i) {
                result = result * (seq_t)10 + (seq_t)digits[i - 1];
            }
        }
        /* Emit the digit itself */
        result = result * (seq_t)10 + (seq_t)d;
        return result;
    }

    seq_t temp = cur;
    while (temp > (seq_t)0) {
        unsigned int d = (unsigned int)(temp % (seq_t)10);
        if (!seen[d]) {
            seen[d] = true;
            unsigned int c = cnt[d];
            /* Emit count c */
            if (c > 0) {
                unsigned int digits[32];
                size_t sz = 0;
                unsigned int temp_c = c;
                while (temp_c > 0) {
                    digits[sz++] = temp_c % 10;
                    temp_c /= 10;
                }
                for (size_t i = sz; i > 0; --i) {
                    result = result * (seq_t)10 + (seq_t)digits[i - 1];
                }
            }
            /* Emit the digit itself */
            result = result * (seq_t)10 + (seq_t)d;
        }
        temp /= (seq_t)10;
    }

    return result;
}

/* --------------------------------------------------------------------- */
/*  Ensure the cache contains terms up to n (inclusive)                 */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;          /* already computed */

    if (cache_size == 0) {               /* initialise cache */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Grow the allocation if necessary */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = compute_next(cache[i - 1]);
    }
    cache_size = n + 1;
}
