/*
 * A032859  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has d(m)
 *          > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032859: Numbers whose base‑4 representation
 * alternates digits starting with '>' at the most significant side.
 * That is, for the digits d(m)…d(0) in base 4 we have
 * d(m) > d(m-1) < d(m-2) > d(m-3) …  (type “> < > < …”).
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */
#include <stdbool.h>  /* bool */
#include "../utils/mem.h"

/*------------------------------------------------------------*/
static bool alternating_base4(seq_t x)
{
    unsigned char digits[128]; /* enough for all practical seq_t sizes */
    size_t len = 0;

    if (x == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x) {
            digits[len++] = (unsigned char)(x & (seq_t)3); /* x % 4 */
            x >>= 2;                                     /* x / 4 */
        }
    }

    /* Pattern: d(m) > d(m-1) < d(m-2) > … */
    for (size_t k = len - 1; k > 0; --k) {
        unsigned char a = digits[k];
        unsigned char b = digits[k - 1];
        if (((len - 1 - k) & 1) == 0) {          /* expect '>' */
            if (!(a > b))
                return false;
        } else {                                 /* expect '<' */
            if (!(a < b))
                return false;
        }
    }
    return true;
}

/*------------------------------------------------------------*/
/* Static state for memoisation                               */
/*------------------------------------------------------------*/
static seq_t *a_cache      = NULL;   /* array of computed terms      */
static size_t a_cache_size = 0;      /* number of terms stored       */
static size_t a_cache_cap  = 0;      /* current capacity of a_cache  */
static seq_t  a_next       = 0;      /* next integer to test         */

/*------------------------------------------------------------*/
/* Grow the cache to hold at least 'need' entries             */
/*------------------------------------------------------------*/
static void grow_cache(size_t need)
{
    if (need <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) {
        /* allocation failure: terminate */
        exit(EXIT_FAILURE);
    }
    a_cache      = tmp;
    a_cache_cap  = new_cap;
}

/*------------------------------------------------------------*/
/* Check whether a number has the required alternating pattern*/
/* in base 4.  Works for all supported seq_t types.           */

/*------------------------------------------------------------*/
/* Public sequence function: a(n)                           */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, return 0 as a placeholder */

    /* Ensure the cache contains at least n terms. */
    while (a_cache_size < (size_t)n) {
        /* Make sure the cache can hold another term. */
        if (a_cache_cap <= a_cache_size)
            grow_cache(a_cache_size + 1);

        /* Find the next term satisfying the alternating pattern. */
        while (!alternating_base4(a_next))
            ++a_next;

        /* Store it and advance to the next candidate. */
        a_cache[a_cache_size++] = a_next;
        ++a_next;
    }

    return a_cache[n - 1];
}
