/*
 * A043976  Numbers k such that 2 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043976: numbers k such that 2 and 4 occur next to each other in
   the base‑9 representation of k but not of k+1.  The function
   builds the sequence incrementally and caches the terms, so that any
   n ≥ 1 can be answered in O(n) time and stays well below the 2‑second
   limit for n up to about 1e4. */
#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool, true, false */
#include "../utils/has_adjacent_24.h"

/* static cache large enough for the required range (n≤≈1e4) */
#define MAX_CACHE 200000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* Return the n‑th term of A043976, offset 1 (a(1)=22). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behaviour below the offset is irrelevant */

    /* ensure that we have generated at least n terms */
    if (cache_len < (size_t)n) {
        seq_t cand = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;
        while (cache_len < (size_t)n) {
            /* find next candidate that satisfies the condition */
            while (true) {
                if (has_adjacent_24(cand) && !has_adjacent_24(cand + 1))
                    break;
                cand++;
            }

            /* store the candidate */
            cache[cache_len++] = cand;
            cand++;  /* start next search just after current candidate */
        }
    }

    return cache[(size_t)n - 1];
}
