/*
 * A043931  Numbers k such that 5 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_5_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implements the sequence A043931:
   A number k is in the sequence if (5,6) or (6,5) occurs
   consecutively in the base‑7 representation of k but not in that of k+1.
   The function generates the sequence by brute‑force search, memoising
   found terms so that subsequent calls do not repeat work.  The
   implementation is fully compliant with C11 and C++17.  (The
   cast of the result of realloc is required for C++.) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_5_6.h"

/* Memoisation storage */
static seq_t *cache         = NULL;    /* array of terms found so far   */
static size_t cache_len    = 0;         /* number of terms stored       */
static size_t cache_cap    = 0;         /* allocated capacity          */
static seq_t next_candidate = 1;       /* next integer to test       */

/* Return the n‑th term (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for offset 0 */
        return (seq_t)0;

    /* Grow the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = next_candidate;        /* candidate number */

        bool k_has  = has_adjacent_5_6(k);
        bool kp1_has = has_adjacent_5_6(k + 1);

        if (k_has && !kp1_has) {
            /* Store the term */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();               /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }

        /* Advance to next candidate */
        ++next_candidate;
    }

    return cache[n - 1];
}
