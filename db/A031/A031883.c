/*
 * A031883  First differences of lucky numbers.
 * https://oeis.org/A031883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n+1) – L(n) where L(k) is the k‑th lucky number (OEIS
 *            A000959)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates lucky numbers on demand and stores them in a static cache.
   It then returns the first difference at the requested index. */

/* Required standard headers */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for lucky numbers */
static seq_t *lucky = NULL;          /* array of lucky numbers, 0‑based: lucky[0] = 1 */
static idx_t lucky_len = 0;          /* number of lucky numbers stored */
static size_t lucky_cap = 0;         /* buffer capacity */

/* Generate lucky numbers until at least 'needed' are available.
   The algorithm builds a list of odd numbers up to a limit,
   then applies the lucky‑number sieve.  It is recomputed from scratch
   each time the cache is insufficient, which is acceptable for n ≤ 10 000. */
static void ensure_lucky_count(idx_t needed)
{
    if (lucky_len >= needed)
        return;

    /* Rough estimate of the range of odd numbers required. */
    idx_t limit = needed * 20 + 10;
    if (limit < 100)
        limit = 100;

    while (1) {
        /* Number of odd integers up to 'limit' */
        idx_t odds_cap = limit / 2 + 1;
        seq_t *odds = (seq_t *)malloc((size_t)odds_cap * sizeof(seq_t));
        if (!odds)
            return; /* allocation failure – give up */

        /* Fill with odd numbers 1,3,5,... */
        seq_t val = 1;
        for (idx_t i = 0; i < odds_cap; ++i, val += 2) {
            odds[i] = val;
        }

        idx_t sz = odds_cap;
        /* Lucky sieve */
        for (idx_t i = 1; i < sz; ++i) {
            seq_t step = odds[i];
            if (step > (seq_t)sz) /* step larger than remaining list: stop */
                break;
            idx_t write = 0, count = 0;
            for (idx_t j = 0; j < sz; ++j) {
                ++count;
                if (count == step) { /* remove this element */
                    count = 0;
                    continue;
                }
                odds[write++] = odds[j]; /* keep this element */
            }
            sz = write;
        }

        /* Store the lucky numbers in the static cache */
        if (lucky_cap < (size_t)sz) {
            seq_t *tmp = (seq_t *)realloc(lucky, (size_t)sz * sizeof(seq_t));
            if (!tmp) { free(odds); return; }
            lucky = tmp;
            lucky_cap = (size_t)sz;
        }
        memcpy(lucky, odds, (size_t)sz * sizeof(seq_t));
        lucky_len = sz;
        free(odds);

        if (lucky_len >= needed)
            break; /* success */

        /* Increase search window and try again */
        if (limit == SIZE_MAX / 2)
            break; /* avoid overflow */
        limit *= 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* invalid index – undefined in OEIS definition */

    /* Ensure we have enough lucky numbers cached */
    ensure_lucky_count(n + 1);

    /* Return the first difference at the requested index */
    return lucky[n] - lucky[n - 1];
}
