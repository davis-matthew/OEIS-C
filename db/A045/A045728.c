/*
 * A045728  Fibonacci numbers having initial digit '4'.
 * https://oeis.org/A045728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th Fibonacci number whose first decimal digit is 4
 *            (OEIS A045728)
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, fib, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper function declarations (provided by the build system) */
#include "../utils/fib.h"         /* seq_t fib(idx_t n); */
#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t exp); */

/* Static cache for values of the sequence */
static seq_t *cache = NULL;    /* array of sequence values */
static size_t cache_cap = 0;   /* allocated capacity of the array */
static size_t cache_len = 0;   /* number of values stored */

/* Index of the next Fibonacci number to test */
static idx_t next_fib_index = 0;

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A045728.  n is 1-based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for indices below the offset */

    /* Build the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t f = fib(next_fib_index);       /* compute F(n) */
        int dlen = digit_len(f);             /* decimal digit length */
        seq_t pow = pow10((idx_t)(dlen - 1)); /* 10^(dlen-1) */
        seq_t leading = f / pow;             /* integer division gives first digit */

        /* Raw value of the leading digit (must be ≤9). */
        unsigned long long ld = (unsigned long long)leading;

        if (ld == 4ULL) {                    /* leading digit is 4 */
            ensure_cache(cache_len + 1);
            cache[cache_len++] = f;
        }
        next_fib_index++;                    /* advance to next Fibonacci index */
    }

    return cache[n - 1];  /* 0‑based array index */
}
