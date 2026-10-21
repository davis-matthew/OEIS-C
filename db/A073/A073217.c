/*
 * A073217  The terms of A055237 (sums of two powers of 5) divided by 2.
 * https://oeis.org/A073217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   compare_seq, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/********************************************************************
 * This function computes the OEIS sequence A073217
 *   a(n) = (5^m + 5^k)/2   with 0 ≤ k ≤ m ,  n≥1  (offset 1)
 *
 * The terms are generated in sorted order by enumerating all pairs
 * (m,k) with m ≥ k, computing (5^m+5^k)/2, sorting the list, and
 * keeping the array in static storage for subsequent calls.
 *
 * For any requested index n ≥ 1 the function returns a(n).
 ********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "../utils/power5.h"
#include "../utils/compare_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    /* static cache of computed terms */
    static seq_t *terms   = NULL;  /* sorted sequence */
    static idx_t  cached  = 0;     /* number of cached terms */

    /* determine how many terms are needed for index n */
    idx_t required = n;
    if (cached >= required)
        return terms[required - 1];

    /* find exponent E so that (E+1)*(E+2)/2 ≥ required */
    idx_t E = 0;
    while ((E + 1) * (E + 2) / 2 < required)
        ++E;

    idx_t pow_len    = E + 1;                 /* #powers of 5 */
    idx_t pair_count = pow_len * (pow_len + 1) / 2; /* #pairs (m,k) with m≥k */

    /* allocate temporary arrays */
    seq_t *powers = (seq_t *)malloc(sizeof(seq_t) * pow_len);
    if (!powers) abort();

    seq_t *pairs  = (seq_t *)malloc(sizeof(seq_t) * pair_count);
    if (!pairs) abort();

    /* compute powers of 5 */
    for (idx_t i = 0; i < pow_len; ++i)
        powers[i] = power5(i);

    /* generate all (5^m + 5^k)/2 values */
    idx_t pos = 0;
    for (idx_t m = 0; m < pow_len; ++m) {
        for (idx_t k = 0; k <= m; ++k) {
            pairs[pos++] = (powers[m] + powers[k]) >> 1;
        }
    }

    /* sort the terms */
    qsort(pairs, pair_count, sizeof(seq_t), compare_seq);

    /* replace cached array with the new sorted list */
    if (terms) free(terms);
    terms = pairs;
    cached = pair_count;

    /* free temporary powers array */
    free(powers);

    return terms[required - 1];
}
