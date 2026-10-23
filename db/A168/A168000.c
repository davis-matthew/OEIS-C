/*
 * A168000  Numbers k with property that first digit of 6*k = last digit of k.
 * https://oeis.org/A168000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th positive integer k such that the first decimal
 *            digit of 6*k equals the last decimal digit of k
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* cache = NULL;          /* array of found terms */
static size_t  cache_cap = 0;        /* capacity of the array */
static idx_t   cache_cnt = 0;        /* how many terms are stored */
static idx_t   cur_k   = 1;          /* next k to test */

/* returns the most significant decimal digit of x */
static inline seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return x;
}

/* Make sure the cache can hold at least n terms.  The cache grows
   geometrically, lazily generating terms only as needed. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cnt) return;

    /* Grow cache array if necessary */
    if (cache_cnt == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failure: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms until we have at least n of them */
    while (cache_cnt < n) {
        seq_t last   = cur_k % (seq_t)10;           /* last decimal digit of k */
        if (last != 0) {                            /* first digit cannot be 0   */
            seq_t t  = (seq_t)cur_k * (seq_t)6;     /* 6*k                          */
            seq_t fd = first_digit(t);             /* first digit of 6*k           */
            if (fd == last)
                cache[cache_cnt++] = cur_k;
        }
        cur_k++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, but harmless */
    ensure_cache(n);               /* guarantee cache has at least n terms */
    return cache[n - 1];           /* array is 0‑indexed but OEIS is 1‑indexed */
}
