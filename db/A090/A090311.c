/*
 * A090311  Concatenation of two distinct primes p and q, p < q.
 * https://oeis.org/A090311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 318, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090311:
   Concatenation of two distinct primes p < q, ordered by increasing
   q then increasing p (i.e. for each q list all p<q, then move to the
   next prime q).  The function generates enough primes on demand and
   figures out the corresponding pair for the requested index n. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* n is 1‑based (offset 1).  Anything less is undefined. */
    if (n < 1) return 0;

    /* static cache of primes */
    static uint32_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;

    /* initialise prime list with the first prime */
    if (primes_len == 0) {
        primes_cap = 32;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) return 0;               /* allocation failure */
        primes[0] = 2;
        primes_len = 1;
    }

    /* compute the q‑index i such that i*(i+1)/2 >= n */
    idx_t i = 0;          /* zero‑based index of the second prime q */
    idx_t cum = 0;        /* cumulative number of pairs up to current i */
    while (cum < n) {
        ++i;
        cum += i;
    }

    /* number of pairs before this q */
    idx_t prev_cum = cum - i;
    /* 1‑based offset of the desired p among the p’s for this q */
    idx_t offset = n - prev_cum;
    /* zero‑based index of the first prime p */
    idx_t p_idx = offset - 1;
    /* ensure we have at least (i+1) primes (indices 0..i inclusive) */
    while (primes_len <= i) {
        /* generate the next prime after primes[primes_len-1] */
        uint32_t candidate = (primes_len == 1) ? 3
                                               : primes[primes_len - 1] + 2;
        while (true) {
            bool prime = true;
            for (size_t j = 1; j < primes_len; ++j) { /* skip 2 */
                uint32_t p = primes[j];
                if ((uint64_t)p * p > candidate) break;
                if (candidate % p == 0) { prime = false; break; }
            }
            if (prime) break;
            candidate += 2;
        }
        /* grow the array if necessary */
        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap << 1;
            uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
            if (!tmp) return 0;               /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_len++] = candidate;
    }

    /* fetch the primes and concatenate them */
    return concat_parts((idx_t)primes[p_idx], (idx_t)primes[i]);
}
