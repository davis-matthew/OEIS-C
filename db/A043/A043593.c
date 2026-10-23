/*
 * A043593  Numbers whose base-4 representation has exactly 2 runs.
 * https://oeis.org/A043593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑4 representation has exactly two runs
 *            (OEIS A043593). The function generates all such numbers up to
 *            the length required by the requested index, sorts them, and
 *            returns the n‑th smallest value (1‑based)
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort – works for any seq_t that supports
   operator <, == after casting the void pointers. */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* A value of 0 is undefined (the sequence is 1‑based). */
    if (n == 0)
        return (seq_t)0;

    /* static state: cache of all generated terms, sorted once per
       generation pass. */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;   /* number of stored terms */
    static size_t cache_cap   = 0;   /* malloced capacity */
    static bool   dirty       = false;          /* new entries added */
    static idx_t  current_len = 1;             /* base‑4 length processed */

    /* Invariant: cache contains all terms of lengths <= current_len,
       sorted if dirty is false. */

    /* Generate terms until we have at least n values. */
    while (cache_count < (size_t)n)
    {
        ++current_len;                /* treat next base‑4 length */

        /* A single digit cannot have two runs, so skip length 1. */
        if (current_len < 2)
            continue;

        int L = (int)current_len;                  /* current length */

        /* For this length L there are 9*(L‑1) new terms:
           3 choices for the first digit, 3 for the second (different),
           and (L-1) split positions. */
        size_t new_terms = 9 * (size_t)(L - 1);

        /* Ensure capacity.  Use doubling strategy. */
        if (cache_cap < cache_count + new_terms)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < cache_count + new_terms)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate all numbers of length L.  The form is
           d^k e^(L‑k) with d in {1,2,3}, e in {0,1,2,3}\{d},
           k=1..L-1. */
        for (int d = 1; d <= 3; ++d)
        {
            for (int e = 0; e <= 3; ++e)
            {
                if (e == d) continue;           /* digits must differ */

                for (int k = 1; k < L; ++k)     /* run length of d */
                {
                    int m = L - k;              /* run length of e */
                    seq_t val = 0;

                    /* first k digits = d */
                    for (int i = 0; i < k; ++i)
                        val = val * ((seq_t)4) + (seq_t)d;

                    /* m digits = e */
                    for (int i = 0; i < m; ++i)
                        val = val * ((seq_t)4) + (seq_t)e;

                    cache[cache_count++] = val;
                }
            }
        }

        dirty = true;      /* new unsorted entries added */
    }

    /* Sort once if we have new entries. */
    if (dirty)
    {
        qsort(cache, cache_count, sizeof(seq_t), cmp_seq_t);
        dirty = false;
    }

    return cache[(size_t)(n - 1)];
}
