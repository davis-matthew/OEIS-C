/*
 * A174503  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A087799(n))
 *          ), where A087799(n) = (5+sqrt(24))^n + (5-sqrt(24))^n.
 * https://oeis.org/A174503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174503
   Recurrence (conjecture proven):
   a(0) = 1, a(1) = 8, a(2) = 1, a(3) = 96, a(4) = 1, a(5) = 968,
   a(n) = 11*a(n-2) – 11*a(n-4) + a(n-6)  for n ≥ 6
   (Odd indices are a(2k+1) = A087799(k+1) – 2, even indices are 1.) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t  cache_len = 0;  /* number of computed values */
static idx_t  cache_cap = 0;  /* allocated capacity */

/* Ensure cache has at least 'needed' + 1 entries (indices 0..needed). */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_len)
        return;

    /* resize if necessary */
    if (needed >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= needed)
            new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) exit(1);   /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }

    /* compute missing entries */
    for (idx_t i = cache_len; i <= needed; ++i) {
        if (i == 0)           cache[0] = (seq_t)1;
        else if (i == 1)      cache[1] = (seq_t)8;
        else if (i == 2)      cache[2] = (seq_t)1;
        else if (i == 3)      cache[3] = (seq_t)96;
        else if (i == 4)      cache[4] = (seq_t)1;
        else if (i == 5)      cache[5] = (seq_t)968;
        else {
            seq_t a2 = cache[i-2];
            seq_t a4 = cache[i-4];
            seq_t a6 = cache[i-6];
            seq_t term = (seq_t)11 * a2;   /* 11*a(n-2) */
            term = term - (seq_t)11 * a4;  /* -11*a(n-4) */
            cache[i] = term + a6;          /* + a(n-6) */
        }
    }
    cache_len = needed + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
