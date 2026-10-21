/*
 * A089988  Possible differences of n^2*(n+1)/2.
 * https://oeis.org/A089988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089988: 
 * a(n) is the n‑th smallest positive value of 
 *     T(n) - T(m),  T(k)=k^2*(k+1)/2,  n>m.
 * The sequence is sorted and duplicates are removed.
 *
 * This implementation generates all differences for all pairs (i,j)
 * with 1≤j<i≤bound, where bound is increased until at least n terms
 * are produced.  The values are stored in a static array and reused
 * across calls.  The algorithm is O(bound^2 log bound); for n≤10000
 * we need bound≈2000, which is comfortably fast on 64‑bit or
 * arbitrary‑precision types.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_cmp.h"

/* static storage for the generated sequence */
static seq_t *seq_array   = NULL;   /* sorted distinct differences */
static size_t seq_len     = 0;      /* number of stored terms       */
static idx_t  max_n_used  = 0;      /* maximum n for which T(n) was computed */

/* regenerate the sequence until it contains at least needed terms */
static void recompute_sequence(idx_t needed)
{
    /* increase bound until we have enough terms */
    while (seq_len < (size_t)needed) {
        /* determine next bound */
        idx_t bound = (max_n_used == 0) ? 100 : max_n_used * 2;

        /* number of directed pairs (i>j) */
        size_t pairs = (size_t)bound * (bound - 1) / 2;

        /* allocate temporary array for all differences */
        seq_t *tmp = (seq_t *)malloc(pairs * sizeof(seq_t));
        if (!tmp) abort();

        /* precompute T(k) for 1..bound */
        seq_t *T = (seq_t *)malloc((bound + 1) * sizeof(seq_t));
        if (!T) abort();
        for (idx_t i = 1; i <= bound; ++i) {
            seq_t nn = (seq_t)i;
            T[i] = nn * nn * (nn + (seq_t)1) / (seq_t)2;
        }

        /* generate all differences T(i)-T(j) for i>j */
        size_t pos = 0;
        for (idx_t i = 2; i <= bound; ++i) {
            seq_t Ti = T[i];
            for (idx_t j = 1; j < i; ++j)
                tmp[pos++] = Ti - T[j];
        }
        free(T);

        /* sort and deduplicate */
        qsort(tmp, pairs, sizeof(seq_t), seq_cmp);
        size_t uniq = 0;
        for (size_t i = 0; i < pairs; ++i) {
            if (i == 0 || tmp[i] != tmp[i - 1])
                tmp[uniq++] = tmp[i];
        }

        /* shrink to the right size */
        seq_t *shrink = (seq_t *)realloc(tmp, uniq * sizeof(seq_t));
        if (!shrink) abort();

        /* replace old array */
        free(seq_array);
        seq_array = shrink;
        seq_len   = uniq;
        max_n_used = bound;
    }
}

/* public function: a(n) with 1‑based indexing */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* ensure we have at least n terms */
    recompute_sequence(n);

    /* n is 1‑based; array is 0‑based */
    return seq_array[n - 1];
}
