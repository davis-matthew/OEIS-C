/*
 * A170596  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of reduced words of length n in the Coxeter group 
   on 11 generators S_i with relations (S_i)^2 = I and (S_i S_j)^48 = I.

   The sequence satisfies the rational generating function

        G(t) = (t^48 + 2 t^47 + ... + 2 t + 1)
/              (1 - 9 t - 9 t^2 - ... - 9 t^47 + 45 t^48).

   From this one obtains the linear recurrence

        a(0) = 1,
        for 1 ≤ n ≤ 47     a(n)   = 2 + 9 * Σ_{i=0}^{n-1} a(i),
        a(48)                = 1 + 9 * Σ_{i=1}^{47} a(i) - 45 * a(0),
        for n ≥ 49          a(n)   = 9 * Σ_{i=n-47}^{n-1} a(i) - 45 * a(n-48).

   The implementation below builds the terms sequentially and caches
   them so that repeated calls in any order are constant‑time after an
   O(n) initial build.  All arithmetic is performed with the type
   `seq_t`, which may be an unsigned integer of any width or an
   arbitrary‑precision integer class.
*/

#include <stdlib.h>

/* cached values of the sequence */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Return a(n).  Directly evaluates the recurrence above and
   extends the cache as needed. */
seq_t a(idx_t n)
{
    /* Extend cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (new_cache == NULL) {
            /* allocation failed – abort gracefully */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Compute all new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sumPrev = 0;
            idx_t start = (i < 47 ? 0 : i - 47);   /* first index to sum */
            for (idx_t j = start; j < i; ++j) {
                sumPrev += cache[j];
            }

            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 47) {
                val = (seq_t)2 + (seq_t)9 * sumPrev;
            } else if (i == 48) {
                val = (seq_t)1 + (seq_t)9 * sumPrev - (seq_t)45 * cache[0];
            } else {
                val = (seq_t)9 * sumPrev - (seq_t)45 * cache[i - 48];
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
