/*
 * A128290  If p(x) is the product of the digits of the number x and s(x) the
 *          sum of the digits then the sequence lists all the numbers k for
 *          which p(s(k)) = s(p(k)), with k >= 1.
 * https://oeis.org/A128290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th integer k ≥ 1 such that *
 *            digit_product(digit_sum(k)) = digit_sum(digit_product(k))
 * Range:     u8 n <= 22, u16 n <= 4918, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9386 (2026-11-03)
 * Helpers:   digit_product, digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates terms on demand. It keeps a static
 * list of already found terms and grows it as needed.            */

#include <stddef.h>                    /* for size_t */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *cache = NULL;       /* array of found terms          */
static size_t cache_len = 0;      /* number of terms stored        */
static size_t cache_cap = 0;      /* current capacity of cache     */
static seq_t next_candidate = 1;  /* next integer to test          */

static int term_is_good(seq_t x)
{
    seq_t s = digit_sum(x);
    seq_t p = digit_product(x);
    seq_t left  = digit_product(s);  /* p(s(x))   */
    seq_t right = digit_sum(p);      /* s(p(x))   */
    return left == right;
}

static void grow_cache(size_t new_cap)
{
    /* Allocate new storage, copy old contents, and free old array. */
    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache) return;           /* allocation failure – abort silently.   */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1, but return 0.     */
    /* Ensure we have at least n terms computed. */
    while (cache_len < (size_t)n) {
        if (term_is_good(next_candidate)) {
            /* Append to cache, expanding storage if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                grow_cache(new_cap);
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n-1];
}
