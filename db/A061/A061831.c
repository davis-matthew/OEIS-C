/*
 * A061831  Multiples of 9 having only even digits.
 * https://oeis.org/A061831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9484 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A061831: multiples of 9 with all decimal digits even.
   a(1)=0, a(n) is the n‑th such number in increasing order. */
#include <stdbool.h>
#include <stdlib.h>
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

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

static seq_t *cache = NULL;          /* cached terms          */
static size_t  cache_len = 0;        /* number of terms cached*/
static size_t  cache_cap = 0;        /* capacity of cache     */
static seq_t   next_candidate = 0;   /* next multiple of 9 to test  */
static bool    initialized = false; /* one‑time init flag     */

seq_t a(idx_t n) {
    if (!initialized) {
        initialized = true;
        /* first term is 0 */
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)0);
        /* start searching from the next multiple of 9 */
        next_candidate = (seq_t)9;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* test current candidate */
        if (all_even_digits(next_candidate))
            append_candidate(&cache, &cache_len, &cache_cap, next_candidate);
        /* advance to next multiple of 9 */
        next_candidate += (seq_t)9;
    }

    return cache[(size_t)(n - 1)];
}
