/*
 * A045275  Numbers whose base-5 representation contains exactly three 2's and
 *          one 3.
 * https://oeis.org/A045275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation contains exactly three 2's and one 3.
   The function generates the terms in increasing order by enumerating
   numbers of length 4,5,6,... in base‑5, keeping values in a cache.
   Each recursive step builds the digits from most significant to least,
   ensuring that all required digits (three 2's and one 3) are placed
   exactly once.  The recursion enumerates children in ascending digit
   order, guaranteeing the overall numeric order.  The cache is enlarged
   lazily until it contains at least n terms.  Subsequent calls reuse
   the cache. */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached terms */
static size_t cache_sz  = 0;       /* number of cached terms   */
static size_t cache_cap = 0;       /* allocated capacity       */
static idx_t   curr_len = 4;       /* length to generate next  */

/* Append a value to the cache, increasing capacity as needed. */
static void add_value(seq_t v)
{
    if (cache_sz == cache_cap) {
        if (cache_cap == 0) cache_cap = 64;
        else cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_sz++] = v;
}

/* Recursively generate all base‑5 numbers of given length that
   contain exactly three 2's and one 3.  `pos` is the current digit
   index (0 is most significant).  `used2` / `used3` count how many
   2's / 3's have already been placed.  Numbers that would
   exceed the required counts are pruned. */
static void gen_rec(idx_t len, idx_t pos, int used2, int used3, seq_t cur)
{
    int rem = (int)(len - pos);          /* remaining digit positions   */
    int need2 = 3 - used2;               /* 2's yet to place            */
    int need3 = 1 - used3;               /* 3's yet to place            */
    if (rem < need2 + need3) return;     /* impossible to fill remaining */

    if (pos == len) {                    /* finished a candidate        */
        if (used2 == 3 && used3 == 1) add_value(cur);
        return;
    }

    for (int d = 0; d <= 4; ++d) {       /* try all digits             */
        if (pos == 0 && d == 0) continue; /* leading zero forbidden      */
        if (d == 2 && used2 == 3) continue; /* too many 2's              */
        if (d == 3 && used3 == 1) continue; /* too many 3's              */
        int nused2 = used2 + (d == 2);
        int nused3 = used3 + (d == 3);
        seq_t new_cur = cur * 5 + (seq_t)d;
        gen_rec(len, pos + 1, nused2, nused3, new_cur);
    }
}

/* Generate all terms of base‑5 length `len` and append them to the cache.
   These values are produced in ascending numeric order. */
static void generate_len(idx_t len)
{
    gen_rec(len, 0, 0, 0, (seq_t)0);
}

/* External entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined offset, harmless  */

    /* Grow cache until it contains at least n terms. */
    while (cache_sz < n) {
        generate_len(curr_len);
        curr_len++;
    }

    return cache[n - 1];
}
