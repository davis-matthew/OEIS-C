/*
 * A043821  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 6 mod 10.
 * https://oeis.org/A043821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer k such that the number of runs
 * in the base‑3 representation of k is congruent to 6 (mod 10).
 * The routine simply scans successive integers and counts digit runs
 * until it has found n terms.  Results are memoised so that repeated
 * calls are fast.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_k     = 1;   /* next integer to test */

    /* if we already have the n‑th term cached */
    if (cache_len >= n)
        return cache[n-1];

    /* initialise cache on first use */
    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until it contains n entries */
    while (cache_len < n) {
        idx_t val = next_k;
        int   runs = 0;
        int   prev_digit = -1;      /* sentinel for "no previous digit" */

        /* count runs in base‑3 representation of val */
        while (val > 0) {
            int digit = val % 3;
            if (digit != prev_digit) {
                runs++;
                prev_digit = digit;
            }
            val /= 3;
        }

        /* store value if it satisfies the congruence condition */
        if ((runs % 10) == 6) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)next_k;
            cache_len++;
        }

        next_k++;
    }

    return cache[n-1];
}
