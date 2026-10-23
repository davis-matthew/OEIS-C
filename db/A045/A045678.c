/*
 * A045678  Number of 2n-bead balanced binary necklaces which are equivalent
 *          to their reversed complement, but not equivalent to their reverse
 *          and complement.
 * https://oeis.org/A045678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(2n+1) = 4^n - 2^n * a(2n) = 2^(2n-1) - 2^n + a(n) for n ≥ 1
 *            * a(0) = 0 * * Uses memoised recursion with helper pow2(idx_t)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"   /* pow2(idx_t) is available */

seq_t a(idx_t n)
{
    /* Memoisation tables */
    static seq_t *memo = NULL;      /* computed values */
    static bool  *done  = NULL;      /* flags if memo[i] is valid */
    static idx_t  cap   = 0;         /* current capacity of tables */

    /* extend tables if necessary */
    if (n >= cap) {
        idx_t new_cap = (n + 1) * 2;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        done = (bool  *)realloc(done,  new_cap * sizeof(bool));
        for (idx_t i = cap; i < new_cap; ++i)
            done[i] = false;
        cap = new_cap;
    }

    /* return cached value if available */
    if (done[n])
        return memo[n];

    seq_t result;

    if (n == 0) {
        result = 0;
    } else if ((n & 1) == 1) {          /* odd: a(2k+1) */
        idx_t k = (n - 1) >> 1;
        seq_t p2k  = pow2(k);            /* 2^k   */
        seq_t p4k  = pow2(2 * k);        /* 2^(2k)=4^k */
        result = p4k - p2k;              /* 4^k - 2^k */
    } else {                            /* even: a(2k) */
        idx_t k = n >> 1;               /* k = n/2 */
        seq_t p2k_1 = pow2(2 * k - 1);   /* 2^(2k-1) */
        seq_t p2k    = pow2(k);          /* 2^k   */
        /* recursive call for a(k) */
        seq_t ak = a(k);
        result = p2k_1 - p2k + ak;
    }

    /* store in tables and return */
    memo[n] = result;
    done[n] = true;
    return result;
}
