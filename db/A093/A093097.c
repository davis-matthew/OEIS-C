/*
 * A093097  Start with a(1) = 3, a(2) = 7; then apply the rule of A093094.
 * https://oeis.org/A093097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: start with a(1)=3, a(2)=7; for each k>=1,
   let p = a(k)*a(k+1).  Append the base‑10 digits of p
   (most significant first) to the sequence.  The digits
   become the terms a(k+2), a(k+3), …  */

/* required headers for dynamic allocation */
#include <stdlib.h>

/* include utilities if needed (not used here) */
/* #include "../utils/digit_len.h" */
/* #include "../utils/pow10.h" */

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of valid terms in cache */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  next_pair = 0;         /* next pair index to process */

static void
ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

static void
push_digit(seq_t d)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = d;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for 0, return 0 */
        return 0;

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 3;         /* a(1) */
        cache[1] = 7;         /* a(2) */
        cache_len = 2;
        next_pair = 0;        /* next pair to process */
    }

    if (n - 1 < cache_len)
        return cache[n - 1];

    /* generate terms until we reach the requested index */
    while (cache_len < n) {
        /* index of the current pair: a(next_pair) , a(next_pair+1) */
        idx_t i = next_pair;
        /* safety: in a correctly formed sequence always exists */
        if (i + 1 >= cache_len)
            break;

        seq_t prod = cache[i] * cache[i + 1];

        /* prod is at most 81 because each term is a digit (0‑9) */
        if (prod < 10) {
            push_digit(prod);
        } else {
            /* prod has two decimal digits: most significant first */
            push_digit(prod / 10);
            push_digit(prod % 10);
        }

        next_pair++;            /* advance to the next pair */
    }

    return cache[n - 1];
}
