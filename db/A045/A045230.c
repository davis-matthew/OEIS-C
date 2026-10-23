/*
 * A045230  Numbers whose base-5 representation contains exactly three 1's and
 *          one 2.
 * https://oeis.org/A045230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045230: numbers whose base‑5 representation contains exactly
   three 1's and one 2.  The terms are generated in increasing order by
   scanning all integers and selecting those that meet the digit condition.
   The sequence is 1‑indexed. */
    
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache shared by all calls to a() */
static seq_t *cache = NULL;        /* array of computed terms              */
static size_t cap = 0;             /* current capacity of cache            */
static size_t cached = 0;          /* number of terms currently cached     */
static idx_t next_scan = 157;      /* next integer to examine for a match  */

/* Ensure the cache has at least 'min_cap' entries */
static void ensure_capacity(size_t min_cap) {
    if (cap >= min_cap) return;
    size_t new_cap = cap ? cap * 2 : min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* allocation failure: abort */
    cache = tmp;
    cap = new_cap;
}

/* Return the nth term (1‑based) of A045230 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe return */

    /* Extend the cache until it contains at least n elements */
    while (cached < n) {
        /* Scan numbers starting at next_scan until a qualifying number
           is found and added to the cache. */
        for (idx_t v = next_scan;; ++v) {
            /* Count 1's and 2's in base‑5 representation of v. */
            int ones = 0, twos = 0;
            idx_t t = v;        /* temporary copy for digit extraction */
            while (t) {
                int d = (int)(t % 5);
                if (d == 1) ++ones;
                else if (d == 2) ++twos;
                t /= 5;
                /* Early exit if counters already exceed limits. */
                if (ones > 3 || twos > 1) break;
            }
            if (ones == 3 && twos == 1) {
                /* Found a term; store it. */
                ensure_capacity(cached + 1);
                cache[cached++] = (seq_t)v;
                next_scan = v + 1;   /* next search starts after this v */
                break;               /* proceed to possibly next term */
            }
        }
    }

    /* Return the requested term.  cache[0] corresponds to a(1). */
    return cache[n - 1];
}
