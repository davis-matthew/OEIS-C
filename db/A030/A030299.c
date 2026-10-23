/*
 * A030299  Decimal representation of permutations of lengths 1, 2, 3, ...
 *          arranged lexicographically.
 * https://oeis.org/A030299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 153, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A030299.
   The term is the decimal representation of the n‑th permutation
   of the sequence 1,2,…,m arranged lexicographically, where m
   is chosen so that the cumulative number of permutations up to m
   is at least n.  For each element of the permutation its decimal
   expansion is concatenated to produce the result. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Convert n to the integer type used for sequence values. */
    seq_t sn = (seq_t)n;

    /* Find the block length m (1‑based) that contains the n‑th term. */
    size_t m = 1;
    seq_t factorial = 1;          /* m!   */
    seq_t cum = 0;                /* Σ_{i=1}^{m-1} i! */

    while (cum + factorial < sn) {
        cum += factorial;
        factorial *= (seq_t)(m + 1);
        ++m;
    }

    /* Index of the desired permutation within the block (0‑based). */
    seq_t permIdx = sn - cum - 1;

    /* Pre‑compute factorials 0! … m! as seq_t values. */
    seq_t *fact = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    fact[0] = 1;
    for (size_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Prepare an array with the remaining numbers to be permuted. */
    unsigned int *rem = (unsigned int *)malloc(m * sizeof(unsigned int));
    for (size_t i = 0; i < m; ++i)
        rem[i] = (unsigned int)(i + 1);

    /* Build the permutation in lexicographic order. */
    unsigned int *perm = (unsigned int *)malloc(m * sizeof(unsigned int));
    for (size_t pos = 0; pos < m; ++pos) {
        seq_t denom = fact[m - pos - 1];                   /* (m‑pos‑1)! */
        seq_t idxS = permIdx / denom;
        size_t idx = (size_t)idxS;                         /* safe: idx < m */

        perm[pos] = rem[idx];

        /* Remove the chosen element from `rem`. */
        for (size_t t = idx; t + 1 < m - pos; ++t)
            rem[t] = rem[t + 1];

        permIdx %= denom;
    }

    /* Concatenate the elements to produce the decimal representation. */
    seq_t result = 0;
    for (size_t i = 0; i < m; ++i) {
        seq_t x   = perm[i];
        int dlen  = digit_len(x);
        result = result * pow10((idx_t)dlen) + x;
    }

    /* Clean up. */
    free(fact);
    free(rem);
    free(perm);

    return result;
}
