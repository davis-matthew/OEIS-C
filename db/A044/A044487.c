/*
 * A044487  Numbers n such that string 0,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   contains04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer n0 (with n1 = 40, n2 = 76, …) for which
   the base‑6 representation of n0 contains the substring “04”
   but the base‑6 representation of n0 + 1 does not.  The function
   caches previously found terms so that repeated or out‑of‑order
   calls remain efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains04.h"

static seq_t *seq_cache = NULL;    /* dynamically allocated array of terms   */
static size_t seq_len   = 0;       /* number of terms stored                */
static size_t seq_cap   = 0;       /* allocated capacity                    */
static seq_t seq_last_inspected = 0; /* last inspected number                 */

/* a(n) : n >= 1 returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 0, but return 0 safely. */

    /* Return cached value if available. */
    if (n <= seq_len)
        return seq_cache[n - 1];

    /* Ensure cache has initial capacity. */
    if (seq_cap == 0) {
        seq_cap = 8;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache)   /* allocation failed */
            return 0;
    }

    /* Incrementally compute terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        seq_last_inspected++;
        seq_t cur = seq_last_inspected;

        if (contains04(cur) && !contains04(cur + 1)) {
            /* Expand storage if needed. */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_t *new_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!new_cache)   /* allocation failed */
                    return 0;
                seq_cache = new_cache;
            }
            seq_cache[seq_len++] = cur;   /* store the term */
        }
    }

    return seq_cache[n - 1];
}
