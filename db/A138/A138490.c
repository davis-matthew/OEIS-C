/*
 * A138490  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 6.
 * https://oeis.org/A138490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A138490 ("Say what you see in previous term, from the
    right, reporting total number for each digit encountered").
    a(0) = 6.
    For n ≥ 1, a(n) is obtained from a(n‑1) by:
      1. Count the occurrences of every decimal digit in a(n‑1).
      2. Scan the digits of a(n‑1) from the right (least‑significant first).
      3. For each distinct digit encountered in that order, output the
         decimal representation of its count followed by the digit.
    Example: a(4)=162113 is produced from a(3)=3116 → one 3, three 1's,
    one 2, one 6 → 13311216.  */

/* Standard headers needed for memory management and bool.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* -------------  Internal helpers ------------- */

/* Compute the term that follows 'prev'.  */
static seq_t compute_next(seq_t prev)
{
    unsigned int cnt[10]      = {0};
    bool        seen[10]      = {false};
    unsigned char order[10];
    size_t      order_len = 0;

    /* Extract digits of 'prev' from right to left,
       count occurrences and remember first occurrence of each digit.  */
    seq_t tmp = prev;
    if (tmp == (seq_t)0)   /* unlikely but kept for completeness */
    {
        cnt[0]      = 1;
        seen[0]     = true;
        order[0]    = 0;
        order_len   = 1;
    }
    else
    {
        while (tmp > (seq_t)0)
        {
            int d = (int)(tmp % (seq_t)10);       /* digit 0..9  */
            cnt[d]++;

            if (!seen[d])
            {
                seen[d]      = true;
                order[order_len++] = (unsigned char)d;
            }
            tmp /= (seq_t)10;
        }
    }

    /* Build the next term by concatenating the count (in decimal)
       and the digit itself for each digit in the recorded order.  */
    seq_t next = (seq_t)0;

    for (size_t i = 0; i < order_len; ++i)
    {
        unsigned int c = cnt[order[i]];      /* count for this digit, ≥1 */

        /* Convert the count to decimal digits (most significant first) */
        char buf[32];
        int  len = 0;
        unsigned int u = c;
        while (u > 0)
        {
            buf[len++] = (char)('0' + (u % 10));
            u /= 10;
        }

        for (int j = len - 1; j >= 0; --j)
            next = next * (seq_t)10 + (seq_t)(buf[j] - '0');

        /* Append the digit itself */
        next = next * (seq_t)10 + (seq_t)order[i];
    }

    return next;
}

/* -------------  Memoised sequence storage ------------- */

static seq_t  *cache      = NULL;   /* array of saved terms */
static size_t  cache_cap  = 0;      /* allocated capacity */
static idx_t   max_cached = (idx_t)-1; /* index of last computed term; -1 means none yet */

/* Grow the cache to accommodate at least 'needed' indices.  */
static void grow_cache(size_t needed)
{
    if (needed + 1 > cache_cap)
    {
        size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
        if (new_cap < needed + 1)
            new_cap = needed + 1;

        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)      /* allocation failed */
        {
            /* In the rare event of a failure the program will abort;
               the build environment ensures enough space for our needs. */
            abort();
        }
        cache      = new_ptr;
        cache_cap  = new_cap;
    }
}

/* -------------  Public sequence function ------------- */

seq_t a(idx_t n)
{
    if (n == 0)      /* initial term given directly */
        return (seq_t)6;

    /* initialise cache on first use */
    if (max_cached == (idx_t)-1)
    {
        grow_cache(0);
        cache[0] = (seq_t)6;
        max_cached = 0;
    }

    /* compute missing terms up to requested index */
    if (n > max_cached)
    {
        grow_cache((size_t)n);
        for (idx_t i = max_cached + 1; i <= n; ++i)
            cache[i] = compute_next(cache[i - 1]);

        max_cached = n;
    }

    return cache[n];
}
