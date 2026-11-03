/*
 * A159815  Numbers n with property that n^2 starts and ends with 49.
 * https://oeis.org/A159815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 923 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159815: numbers n such that n^2 starts and ends with 49. */
/* The function generates terms in increasing order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static size_t      cache_len = 0;      /* number of terms stored      */
    static size_t      cache_cap = 0;      /* current capacity of array   */
    static seq_t       *cache    = NULL;   /* array of terms              */
    static seq_t       next     = 7;       /* next integer to test        */

    if (n == 0) return 0;      /* undefined for n = 0, return 0 */

    /* Generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* search for the next integer whose square satisfies the property */
        for (;;) {
            seq_t sq = next * next;
            if ((sq % (seq_t)100) == (seq_t)49) {
                size_t d = num_digits(sq);
                if (d >= 2) {
                    seq_t div  = pow10((idx_t)(d - 2));
                    seq_t lead = sq / div;
                    if (lead == (seq_t)49) {
                        /* store the found term */
                        append_candidate(&cache, &cache_len, &cache_cap, next);
                        ++next;        /* move to the next candidate */
                        break;         /* proceed to generate the next cache entry */
                    }
                }
            }
            ++next;                 /* try the following integer */
        }
    }

    return cache[(size_t)n - 1];
}
