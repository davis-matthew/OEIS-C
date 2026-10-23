/*
 * A043344  Numbers having four 2's in base 4.
 * https://oeis.org/A043344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly four 2's in base‑4 representation (A043344). */
/* The sequence is defined by enumerating in increasing order all integers n
   whose base‑4 expansion contains exactly four digits equal to 2. */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoised answers. */
static seq_t *cache = NULL;      /* array of previously computed terms */
static size_t cache_len = 0;     /* number of elements stored in cache   */
static size_t cache_cap = 0;     /* current capacity of the array         */
static int next_len = 4;         /* smallest number of digits needed (4 2's) */

/* Ensure the cache can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Recursive enumeration of all length‑'len' base‑4 numbers that
   contain exactly 'rem2' remaining digits equal to 2.  'pos' is the
   current position (0 = most significant).  'val' holds the number
   constructed so far. */
static void gen_len(int len, int pos, int rem2, seq_t val)
{
    if (pos == len) {
        if (rem2 == 0)
            cache[cache_len++] = val;
        return;
    }
    int positions_left = len - pos - 1;
    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0)        /* no leading zero */
            continue;
        if (rem2 - (d == 2) < 0)
            continue;                 /* too many twos already selected */
        if (rem2 - (d == 2) > positions_left)
            continue;                 /* not enough positions left for remaining twos */
        seq_t new_val = val * (seq_t)4 + (seq_t)d;
        gen_len(len, pos + 1, rem2 - (d == 2), new_val);
    }
}

/* Return the nth term of sequence A043344 (1‑based index). */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call. */
    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache_len = 0;
        next_len = 4;          /* minimal length containing four 2's */
    }

    /* If we already have the requested term, return it. */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)(n - 1)];
    }

    /* Generate terms until we reach the required index. */
    while ((size_t)n > cache_len) {
        grow_cache(cache_len + 1000);   /* give enough space for new terms    */
        gen_len(next_len, 0, 4, (seq_t)0);
        ++next_len;                     /* proceed to the next length       */
    }

    return cache[(size_t)(n - 1)];
}
