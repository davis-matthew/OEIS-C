/*
 * A039292  Numbers whose base-5 representation has the same nonzero number of
 *          3's and 4's.
 * https://oeis.org/A039292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑5 representation has the same nonzero number
 *            of 3's and 4's
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Return true iff x has the same non‑zero number of digits 3 and 4 in base‑5. */
static inline bool has_equal_nonzero_3s_and_4s(seq_t x)
{
    seq_t y = x;
    int c3 = 0, c4 = 0;
    do {
        uint8_t d = (uint8_t)(y % 5);
        if (d == 3) c3++;
        else if (d == 4) c4++;
        y /= 5;
    } while (y);
    return c3 > 0 && c3 == c4;
}

/* Memoised storage of the sequence. */
static seq_t *cache          = NULL;  /* array of sequence values */
static size_t cache_len      = 0;      /* number of values stored */
static size_t cache_cap      = 0;      /* allocated capacity */

/* State used while scanning natural numbers. */
static seq_t   last_tested  = 0;      /* last number examined (>=1) */
static idx_t   matched_cnt  = 0;      /* how many sequence terms found */

/* Return the n-th term, 1‑based. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;  /* undefined for n <= 0, but guard */

    /* If we already have the term, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate more terms until we reach index n. */
    while (matched_cnt < n) {
        ++last_tested;  /* next natural number */

        if (has_equal_nonzero_3s_and_4s(last_tested)) {
            /* Store the new term. */
            append_candidate(&cache, &cache_len, &cache_cap, last_tested);
            ++matched_cnt;
        }
    }

    return cache[(size_t)n - 1];
}
