/*
 * A210015  In base 4, numbers n which have 4 distinct digits, do not start
 *          with 0, and have property that the product (written in base 4) of
 *          any two adjacent digits is a substring of n.
 * https://oeis.org/A210015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210015 – numbers in base 4 with all four digits, no leading 0,
   and for every adjacent pair the base‑4 product is a substring of
   the number.  The sequence elements are quoted as base‑4 digit
   strings written in decimal.  This routine builds the terms in
   numeric order.  It keeps an internal cache and expands it until
   the requested index is reached; subsequent calls reuse the cache. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static size_t cache_size = 0;    /* number of cached terms   */
static size_t cache_cap = 0;     /* allocated capacity       */

/* number of terms already generated (maximum length processed) */
static size_t processed_len = 0; /* 0 means nothing processed yet */

/* list of base‑4 representations of products 0–9 */
static const char *prod_str[10] = {
    "0","1","2","3",
    "10","11","12","13",
    "20","21"
};
static const unsigned char prod_len[10] = {
    1,1,1,1,
    2,2,2,2,
    2,2
};

/* add a new value to the cache, expanding storage if necessary */
static void add_to_cache(seq_t val)
{
    if (cache_size >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

/* global digit buffer (max length 64 is more than enough) */
static int digits[64];

/* recursive generation of all length‑L strings with first digit≠0,
   calling add_to_cache for each valid candidate. */
static void dfs_gen(size_t L, size_t pos, int mask)
{
    if (pos == L) {
        if (mask != 0x0F) /* not all four digits appear */
            return;

        /* build table of present two‑digit substrings */
        bool pair[4][4] = {false};
        for (size_t i = 0; i + 1 < L; ++i)
            pair[digits[i]][digits[i + 1]] = true;

        /* test the product‑substring condition */
        bool ok = true;
        for (size_t i = 0; i + 1 < L && ok; ++i) {
            int prod = digits[i] * digits[i + 1]; /* 0..9 */
            if (prod < 4) { /* one‑digit product */
                if (!(mask & (1 << prod)))
                    ok = false;
            } else { /* two‑digit product in base 4 */
                int d1 = prod >> 2;   /* prod / 4 */
                int d2 = prod & 3;    /* prod % 4 */
                if (!pair[d1][d2])
                    ok = false;
            }
        }
        if (!ok)
            return;

        /* convert digit string to decimal value */
        seq_t val = 0;
        for (size_t i = 0; i < L; ++i)
            val = val * (seq_t)10 + (seq_t)digits[i];

        add_to_cache(val);
        return;
    }

    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0) /* forbid leading zero */
            continue;
        digits[pos] = d;
        dfs_gen(L, pos + 1, mask | (1 << d));
    }
}

/* expand the cache until it contains at least n terms */
static void ensure_terms(idx_t n)
{
    while (cache_size < (size_t)n) {
        size_t L = processed_len ? processed_len + 1 : 4; /* next length */
        dfs_gen(L, 0, 0);
        processed_len = L;
    }
}

/* sequence entry function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n - 1];
}
