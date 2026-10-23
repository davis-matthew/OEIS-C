/*
 * A039374  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 5's.
 * https://oeis.org/A039374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑9 representation has the same nonzero
 * number of 2's and 5's.  The function generates terms by brute‑force
 * search, storing the results in a cache so subsequent calls are fast. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Count whether a number has equal non‑zero numbers of 2 and 5 in base 9. */
static bool is_candidate(seq_t x)
{
    seq_t tmp = x;
    seq_t c2 = 0, c5 = 0;
    while (tmp > 0) {
        seq_t d = tmp % 9;
        if (d == 2)
            ++c2;
        else if (d == 5)
            ++c5;
        tmp /= 9;
    }
    return (c2 > 0) && (c2 == c5);
}

/* Cache for already generated terms. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t  last_checked = 0; /* last integer checked */

/* Main function that returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 0, but safety */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Find the next candidate. */
        do {
            ++last_checked;
        } while (!is_candidate((seq_t)last_checked));

        /* Grow cache if necessary. */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap <= cache_size + 1)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort(); /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found term. */
        cache[cache_size++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
