/*
 * A043624  Numbers whose base-8 representation has exactly 2 runs.
 * https://oeis.org/A043624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates base‑8 integers whose representation has exactly two runs.
   The function caches generated terms and refills the cache only when
   the requested index exceeds the current size. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* Cached sequence values   */
static size_t cache_len = 0;       /* Number of values cached */

/* Comparison routine that works for both built‑in integer types and
   arbitrary‑precision integer classes that provide the usual <, > operators. */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; values below 1 are irrelevant. */
    if (n == 0) return 0;

    /* If we already have enough terms, return directly. */
    if (cache_len < (size_t)n)
    {
        /* Find the smallest base‑8 length L such that the total number
           of two‑run numbers of lengths ≤ L is at least n. */
        idx_t L = 2;                       /* minimum length with two runs */
        size_t cumulative = 0;             /* total terms up to current L */

        while (cumulative < (size_t)n)
        {
            cumulative += 49 * ((size_t)L - 1);
            ++L;
        }
        idx_t max_len = L - 1;             /* last useful length */

        /* Allocate space for all numbers up to max_len. */
        seq_t *tmp = (seq_t *)malloc(cumulative * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */

        /* Generate all two‑run numbers for lengths 2 … max_len. */
        size_t pos = 0;
        for (idx_t len = 2; len <= max_len; ++len)
        {
            for (idx_t split = 1; split < len; ++split)
            {
                for (int a_digit = 1; a_digit <= 7; ++a_digit)
                {
                    for (int b_digit = 0; b_digit <= 7; ++b_digit)
                    {
                        if (b_digit == a_digit) continue;

                        seq_t num = 0;
                        for (idx_t i = 0; i < len; ++i)
                        {
                            int digit = (i < split) ? a_digit : b_digit;
                            num = num * 8 + digit;
                        }
                        tmp[pos++] = num;
                    }
                }
            }
        }

        /* Sort numerically. */
        qsort(tmp, cumulative, sizeof(seq_t), seq_compare);

        /* Replace old cache. */
        if (cache) free(cache);
        cache     = tmp;
        cache_len = cumulative;
    }

    return cache[(size_t)n - 1];
}
