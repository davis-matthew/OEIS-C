/*
 * A115993  Size |S| of the largest subset S of {0,1}^n whose measure m(S) is
 *          <= 2^n, where m is the additive measure defined on each element x
 *          of S by m({x}) = 2^k(x), where k(x) is the number of non-null
 *          coordinates of x.
 * https://oeis.org/A115993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm: greedy selection of vectors from lowest to highest
 *  support.  For each k from 0 upward we take as many vectors with
 *  k ones as possible, until the total weight 2^k per vector would
 *  exceed the budget 2^n.  This policy is optimal because a vector
 *  of smaller support always has a strictly smaller weight.
 *
 *  The function returns a(n), the maximum cardinality of a subset of
 *  {0,1}^n whose weighted sum is at most 2^n. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t limit     = pow2(n);       /* 2^n  */
    seq_t remaining = limit;         /* remaining budget  */
    seq_t count     = 0;             /* number of selected vectors */

    seq_t bin     = 1;   /* C(n,0) */
    seq_t weight  = 1;   /* 2^0   */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* maximum number of vectors of this support that fit */
        seq_t max_take = remaining / weight;

        if (max_take >= bin)
        {
            count += bin;
            /* remaining -= bin * weight; product fits in the budget */
            seq_t prod = bin * weight;
            remaining -= prod;
            if (remaining == 0)
                break;
        }
        else
        {
            count += max_take;
            break;
        }

        /* prepare next k */
        if (k < n)
        {
            /* bin = bin * (n - k) / (k + 1) */
            seq_t mult  = bin * (seq_t)(n - k);
            bin = mult / (seq_t)(k + 1);
            weight = weight * 2;   /* next power of two */
        }
    }
    return count;
}
