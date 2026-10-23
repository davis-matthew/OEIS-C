/*
 * A048325  Numbers whose base-14 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A048325:  Numbers whose base‑14 expansions,
    read from left to right, have run lengths that strictly decrease.
    The function generates the sequence by testing each integer in order
    (starting from 0) and storing the good ones in a static cache.
    The sequence is therefore computed incrementally and is correct
    for any order of calls.  */
#include <stdlib.h>
#include <stdbool.h>

/*  Cache for the computed terms.  */
static seq_t *cache      = NULL;   /* array of values */
static size_t cache_cap  = 0;      /* capacity of the array */
static size_t cache_len  = 0;      /* number of values stored */
static idx_t next_candidate = 0;   /* next integer to test */

/*  Ensure cache has at least 'need' slots.  */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();            /* allocation failure */
    cache     = new_cache;
    cache_cap = new_cap;
}

/*  Return true if the base‑14 representation of 'num' has strictly
    decreasing run lengths (runs of equal consecutive digits).  */
static bool good_base14(seq_t num)
{
    /* Extract digits (least significant first).  */
    unsigned char digits[128];
    int pos = 0;
    seq_t t = num;
    do {
        digits[pos++] = (unsigned char)(t % 14);
        t /= 14;
    } while (t != 0);

    /* Scan from most significant to least, building run lengths.  */
    int last_len = -1;          /* length of previous run */
    int curr_len = 1;           /* length of current run */
    unsigned char curr_digit = digits[pos - 1];

    for (int i = pos - 2; i >= 0; --i) {
        unsigned char d = digits[i];
        if (d == curr_digit) {
            ++curr_len;
        } else {
            if (last_len >= 0 && last_len <= curr_len)
                return false;  /* not strictly decreasing */
            last_len   = curr_len;
            curr_digit = d;
            curr_len   = 1;
        }
    }
    /* Final run check.  */
    if (last_len >= 0 && last_len <= curr_len)
        return false;
    return true;
}

/*  Generate terms until 'cache_len' > need (i.e. cache contains at
    least need + 1 terms).  */
static void generate_terms_up_to(size_t need)
{
    ensure_cache_capacity(need + 1);
    while (cache_len <= need) {
        if (good_base14((seq_t)next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if ((size_t)n >= cache_len)
        /* Need at least n + 1 stored terms.  */
        generate_terms_up_to((size_t)n);
    return cache[(size_t)n];
}
