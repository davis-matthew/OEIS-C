/*
 * A212718  Denominator of Sum_{k=1..n} 1/sigma(k).
 * https://oeis.org/A212718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of \sum_{k=1..n} 1/sigma(k)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   extend_seqt, extend_uint64, gcd, recompute_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/extend_seqt.h"
#include "../utils/extend_uint64.h"
#include "../utils/recompute_sigma.h"

seq_t a(idx_t n)
{
    /* Static state ---------------------------------------------------- */
    static idx_t computed_max = 0;          /* largest index computed so far   */
    static idx_t sigma_size   = 0;          /* current allocated size of sigma */
    static uint64_t *sigma     = NULL;      /* sigma[1..sigma_size-1]          */
    static idx_t denom_size   = 0;          /* current allocated size of denom  */
    static seq_t *denom       = NULL;       /* denom[1..denom_size-1]          */
    static seq_t curr_num     = 0;          /* numerator of current sum        */
    static seq_t curr_den     = 1;          /* denominator of current sum      */

    /* make sure our arrays are large enough (index 0 is unused) */
    idx_t new_cap = n + 1;
    extend_uint64(&sigma,   &sigma_size, new_cap);
    extend_seqt(&denom,     &denom_size, new_cap);

    /* if the requested n is already prepared, return it */
    if (n <= computed_max)
        return denom[n];

    /* Recreate divisor sums up to the new maximum n.
       This guarantees that sigma[1..n] are correct. */
    recompute_sigma(sigma, n);

    /* Append new terms for k = computed_max+1 .. n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t sigma_k = (seq_t)sigma[k];

        /* new_num = curr_num * sigma_k + curr_den          */
        seq_t new_num = curr_num * sigma_k + curr_den;

        /* new_den = curr_den * sigma_k                     */
        seq_t new_den = curr_den * sigma_k;

        /* reduce fraction */
        seq_t g = gcd(new_num, new_den);
        curr_num = new_num / g;
        curr_den = new_den / g;

        /* store the reduced denominator */
        denom[k] = curr_den;
    }

    computed_max = n;
    return denom[n];
}
