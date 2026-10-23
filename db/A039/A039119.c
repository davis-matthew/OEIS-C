/*
 * A039119  Numbers whose base-10 representation has the same number of 0's
 *          and 8's.
 * https://oeis.org/A039119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th natural number whose decimal digits contain
 * an equal number of 0's and 8's.
 * The function builds the sequence lazily and keeps a dynamic cache
 * that is safe for any seq_t type (8, 16, 32, 64, 128 bit unsigned or
 * arbitrary‑precision).
 *
 * Computes each term by scanning successive integers, counting 0's and 8's
 * digit by digit (O(number of digits)), and appends to cache when a term
 * is found.  The cache grows as needed; each call may be in any order.
 */
#include <stddef.h>
#include <stdlib.h>

/* dynamic cache of found terms */
static seq_t *cache = NULL;       /* array holding terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* capacity of the array */

/* next integer to test */
static seq_t next_candidate = 1;

/* ensure cache has at least 'min_len' terms, growing array if needed */
static void grow_cache(size_t min_len)
{
    if (cache_cap >= min_len)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < min_len)
        new_cap = min_len;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failure – abort the program (no other recovery)
         * The OEIS harness expects terminable failures on bad alloc.
         */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* build cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* count zeros and eights in next_candidate */
        unsigned c0 = 0, c8 = 0;
        seq_t tmp = next_candidate;
        while (tmp) {                     /* skip when tmp==0 (never occurs) */
            unsigned d = (unsigned)(tmp % 10);
            if (d == 0) ++c0;
            else if (d == 8) ++c8;
            tmp /= 10;
        }
        if (c0 == c8) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined – return 0 for safety */
    ensure_cache(n);
    return cache[n - 1];
}
