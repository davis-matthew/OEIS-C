/*
 * A180564  Number of permutations of [n] having no isolated entries. An entry
 *          j of a permutation p is isolated if it is not preceded by j-1 and
 *          not followed by j+1. For example, the permutation 23178564 has 2
 *          isolated entries: 1 and 4.
 * https://oeis.org/A180564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..⌊n/2⌋} binomial(n-j-1, j-1) * ( d(j) + d(j-1) )
 *            where d(i) = derangement number A000166(i)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 24, u64 n <= 39, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies:  ../utils/binom.h  defines static seq_t binom(idx_t a, idx_t b) */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n = 0 -> 1, n = 1 -> 0 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* upper bound for j */
    const idx_t max_j = n / 2;

    /* --------- derangement table (d(0) .. d(max_j)) --------- */
    static seq_t *der = NULL;      /* der[i] = d(i) */
    static idx_t der_len = 0;      /* current number of stored values */

    if (der_len <= max_j) {
        /* need to grow table to max_j+1 entries */
        const idx_t new_len = max_j + 1;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;   /* allocation failure */

        /* copy existing values, if any */
        for (idx_t i = 0; i < der_len; ++i) tmp[i] = der[i];

        /* compute missing entries */
        for (idx_t i = der_len; i < new_len; ++i) {
            if (i == 0)                tmp[i] = (seq_t)1;          /* d(0) */
            else if (i == 1)           tmp[i] = (seq_t)0;          /* d(1) */
            else {
                /* d(i) = (i-1) * ( d(i-1) + d(i-2) ) */
                seq_t sum = tmp[i-1] + tmp[i-2];
                tmp[i] = (seq_t)(i-1) * sum;
            }
        }

        /* replace old table */
        free(der);
        der = tmp;
        der_len = new_len;
    }

    /* --------- evaluate the sum --------- */
    seq_t result = 0;
    for (idx_t j = 1; j <= max_j; ++j) {
        seq_t bin = binom( (idx_t)(n - j - 1), (idx_t)(j - 1) );
        seq_t temp = der[j] + der[j-1];
        result += bin * temp;
    }
    return result;
}
