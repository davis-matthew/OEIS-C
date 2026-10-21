/*
 * A229251  Number of set partitions of {1,...,n} with largest set of size 9.
 * https://oeis.org/A229251
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 18, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A229251 – Number of set partitions of {1,…,n} with largest block size 9
 *
 *  a(n) = B_9(n) – B_8(n)  (n ≥ 9)
 *
 *  where B_k(n) is the number of partitions of an n‑element set into blocks
 *  of size at most k, defined by   B_k(0) = 1  and
 *      B_k(n) = Σ_{j=1}^{min(k,n)} C(n-1, j-1) · B_k(n-j).
 *  For n < 9 the function returns 0, as the OEIS offset is 9.
 *
 *  Implementation uses dynamic arrays that grow only when a larger n is
 *  requested.  The arrays are allocated with seq_alloc_new_array and
 *  freed with seq_free_array, ensuring correct handling of any seq_t type
 *  (plain integer or arbitrary‑precision).  The algorithm runs in
 *  O(n) time with a small constant factor (k = 9).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/*------------------------------------------------------------*
 *  Small binomial coefficient C(n,k) for k ≤ 8.              *
 *------------------------------------------------------------*/
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use the smaller k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/*------------------------------------------------------------*
 *  Main accessor – returns a(n) for n ≥ 9, and 0 otherwise. *
 *------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* values are only meaningful for n ≥ 9; for smaller n we return 0. */
    if (n < 9) return 0;

    static seq_t *B8 = NULL;      /* restricted Bell numbers with blocks ≤8 */
    static seq_t *B9 = NULL;      /* restricted Bell numbers with blocks ≤9 */
    static size_t capacity = 0;   /* current allocated length of the arrays   */
    static idx_t  computed  = 0;  /* largest n that has already been computed*/

    /* grow the arrays if we need more space */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        /* allocate new arrays that construct seq_t objects properly */
        seq_t *new_B8 = (seq_t *)seq_alloc_new_array(new_cap);
        seq_t *new_B9 = (seq_t *)seq_alloc_new_array(new_cap);

        /* copy old data */
        for (size_t i = 0; i < capacity; ++i) {
            new_B8[i] = B8[i];
            new_B9[i] = B9[i];
        }

        /* free old arrays if any */
        if (capacity > 0) {
            seq_free_array(B8);
            seq_free_array(B9);
        }

        B8 = new_B8;
        B9 = new_B9;
        capacity = new_cap;

        /* initialise base case only when very first allocation occurs */
        if (capacity == new_cap && new_cap > 0 && new_cap == (size_t)n + 1 && computed == 0) {
            B8[0] = B9[0] = 1;
        }
    }

    /* initialise base case if not already done (first call) */
    if (capacity > 0 && computed == 0) {
        B8[0] = B9[0] = 1;
    }

    /* compute missing entries up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum8 = 0;
        idx_t lim8 = (i < 8 ? i : 8);
        for (idx_t j = 1; j <= lim8; ++j) {
            seq_t c   = binom_small(i - 1, j - 1);
            sum8 = sum8 + c * B8[i - j];
        }
        B8[i] = sum8;

        seq_t sum9 = 0;
        idx_t lim9 = (i < 9 ? i : 9);
        for (idx_t j = 1; j <= lim9; ++j) {
            seq_t c   = binom_small(i - 1, j - 1);
            sum9 = sum9 + c * B9[i - j];
        }
        B9[i] = sum9;
    }
    computed = n;

    /* a(n) = B9(n) – B8(n) */
    return B9[n] - B8[n];
}
