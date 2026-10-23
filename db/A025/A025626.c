/*
 * A025626  Numbers of form 6^i*7^j, with i, j >= 0.
 * https://oeis.org/A025626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 83, u64 n <= 306,
 *            u128 n <= 1177, big any n
 * Helpers:   mul_small, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025626 – sorted numbers of the form 6^i * 7^j (i,j ≥ 0) */

/* 
   The function builds the sequence on demand and caches all
   previously generated terms so that it works for repeated and
   out‑of‑order requests.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"   /* big_pow for small factors */
#include "../utils/seq_cmp.h"     /* comparison of seq_t values */

seq_t a(idx_t n)
{
    /* cache of previously computed terms (static storage) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored */

    /* indices used in the merge algorithm */
    static idx_t i6 = 0, i7 = 0;
    static bool  init = false;

    /* first call – initialise cache with the first term */
    if (!init) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure */
        cache[0] = (seq_t)1;                  /* 6^0 * 7^0 = 1 */
        len     = 1;
        i6      = 0;
        i7      = 0;
        init    = true;
    }

    /* grow the cache if we need to compute more terms */
    while (len < (size_t)n) {
        if (len >= cap) {                     /* double capacity */
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute the next candidate values from the two sequences */
        seq_t next6 = mul_small(cache[i6], 6);
        seq_t next7 = mul_small(cache[i7], 7);

        /* select the smaller (or both when equal) and advance pointers */
        int cmp = seq_cmp(&next6, &next7);
        seq_t next;
        if (cmp == 0) {
            next = next6;
            ++i6;  ++i7;
        } else if (cmp < 0) {
            next = next6;
            ++i6;
        } else {
            next = next7;
            ++i7;
        }

        cache[len++] = next;                  /* store new term */
    }

    /* return the requested term (indices are 1‑based) */
    return cache[n - 1];
}
