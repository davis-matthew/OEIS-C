/*
 * A080676  a(1) = 1; for n>1, a(n) is the smallest number > a(n-1) such that
 *          the first n terms of the sequence contain a total of a(n) digits.
 * https://oeis.org/A080676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A080676: a(1)=1; for n>1, a(n) is the smallest integer >a(n-1)
 *  such that a(n)=∑_{k=1..n} digits(a(k)).
 *  That is, a(n) equals the total number of decimal digits in the first n terms.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache to allow arbitrary call order */
static seq_t *a_cache      = NULL;   /* 1‑based cache of computed terms          */
static size_t cache_cap    = 0;      /* number of allocated elements             */
static idx_t  cached_up_to = 0;      /* largest n for which a_cache[n] is valid   */
static seq_t digit_sum     = 0;      /* sum of digits of all computed terms       */
static seq_t last_val      = 0;      /* a(cached_up_to)                           */

/* ---------- helper: number of decimal digits of a non‑zero number ---------- */
static int digit_len(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x);
    return len;
}

/* ---- fill the cache up to index n (inclusive) -- */
static void ensure_cache(idx_t n)
{
    if (cached_up_to >= n)
        return;

    /* (Re)allocate the cache if needed */
    if (a_cache == NULL) {
        cache_cap = (size_t)n + 1;          /* 1‑based indexing + 1 */
        a_cache   = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!a_cache)
            exit(EXIT_FAILURE);
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        a_cache   = (seq_t *)realloc(a_cache, cache_cap * sizeof(seq_t));
        if (!a_cache)
            exit(EXIT_FAILURE);
    }

    /* initialise first term if cache was empty */
    if (cached_up_to == 0) {
        a_cache[1]   = 1;
        last_val     = 1;
        digit_sum    = 1;   /* digits("1") = 1 */
        cached_up_to = 1;
    }

    /* compute subsequent terms */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t cand;
        int    L = 1;
        /* find smallest valid candidate */
        while (1) {
            cand = digit_sum + (seq_t)L;   /* m = S + digits(m)  */
            if (cand <= last_val) {        /* must be > previous term */
                ++L;
                continue;
            }
            if (digit_len(cand) == L)      /* check digit length */
                break;
            ++L;
        }
        a_cache[i] = cand;
        last_val   = cand;
        digit_sum += digit_len(cand);
        cached_up_to = i;
    }
}

/* ---------- public function required by the problem ---------- */
seq_t a(idx_t n)
{
    if (n < 1)           /* undefined for n<1, but return 0 for safety */
        return 0;
    ensure_cache(n);
    return a_cache[n];
}
