/*
 * A141810  Irregular table: Row n (of A001221(n) terms, for n>=2) consists of
 *          the largest powers that divides n of each distinct prime that
 *          divides n. Terms are arranged by the sizes of the prime-powers.
 *          Row 1 = (1).
 * https://oeis.org/A141810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141810: concatenate rows of unitary prime‑power divisors.
   a(1) = 1; for n ≥ 2, row n consists of pᵉ where pᵉ is the largest
   power of each distinct prime factor of n.  The terms are listed
   row by row in ascending order of their sizes.  This function
   returns the n‑th term of that concatenation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_uint.h"     /* pow_uint(uint64_t base, idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* cumulative term counts: cum[i] = total number of terms up to row i */
    static idx_t *cum   = NULL;   /* cumulative array (1‑based, cum[0] = 0) */
    static size_t cum_size = 0;   /* allocated size (number of rows)      */
    static idx_t cum_len  = 0;    /* highest row number computed          */

    /* initialise the cumulative array on first call */
    if (cum == NULL) {
        cum_size = 128;
        cum = (idx_t *)malloc((cum_size + 1) * sizeof(idx_t));
        cum[0] = 0;               /* no terms before row 1 */
    }

    /* extend the cumulative array until it covers the requested index */
    while (cum_len < n) {          /* cum_len is current max row number computed */
        idx_t row = cum_len + 1;   /* next row to process */
        idx_t len;                 /* number of terms in this row */

        if (row == 1) {
            len = 1;               /* row 1 contains only the single term 1 */
        } else {
            /* factorise row to find distinct primes and their powers */
            uint64_t primes[32];
            uint64_t exps  [32];
            size_t cnt     = 0;
            factor_u64((uint64_t)row, primes, exps, &cnt);
            len = (idx_t)cnt;      /* one term per distinct prime factor */
        }

        /* enlarge the cumulative array if necessary */
        if (cum_len + 1 >= (idx_t)cum_size) {
            size_t new_size = cum_size << 1;
            if (new_size < cum_size + 1) new_size = cum_size + 1;
            cum = (idx_t *)realloc(cum, (new_size + 1) * sizeof(idx_t));
            cum_size = new_size;
        }

        /* store cumulative count */
        cum_len++;
        cum[cum_len] = cum[cum_len - 1] + len;
    }

    /* binary search to locate the row that contains the n‑th term */
    idx_t low  = 1, high = cum_len;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum[mid] >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t row   = low;                     /* row that contains a(n) */
    idx_t prev  = cum[row - 1];            /* total terms before this row */
    idx_t offset = n - prev - 1;           /* 0‑based index within the row */

    /* special handling for the first row */
    if (row == 1)
        return (seq_t)1;

    /* compute all the prime powers in this row */
    uint64_t primes[32];
    uint64_t exps  [32];
    size_t cnt     = 0;
    factor_u64((uint64_t)row, primes, exps, &cnt);

    seq_t values[32];
    for (size_t i = 0; i < cnt; ++i) {
        /* pow_uint returns seq_t, exponent is idx_t */
        values[i] = pow_uint(primes[i], (idx_t)exps[i]);
    }

    /* sort the values ascendingly (cnt is small, trivial O(c²)) */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = values[i];
        size_t j = i;
        while (j > 0 && values[j - 1] > key) {
            values[j] = values[j - 1];
            --j;
        }
        values[j] = key;
    }

    return values[(size_t)offset];
}
