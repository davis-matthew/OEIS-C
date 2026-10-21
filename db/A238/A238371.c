/*
 * A238371  a(1)=1; for n > 1, a(n) = the number of "topped" Mongean shuffles
 *          to reorder a stack of n cards to its original order.
 * https://oeis.org/A238371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   build_primes_upto, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of topped Mongean shuffles needed to restore a pile of n cards
 * (the order of the permutation induced by the shuffle).
 * 
 * The shuffle: 
 *   - the 1st card (index 0) is placed on top,
 *   - the 2nd card (index 1) is placed on bottom,
 *   - thereafter cards are alternately placed on top (even indices)
 *     and on bottom (odd indices).
 * The permutation P has the explicit form:
 *   let m = (n + 1) / 2;          // number of even indices
 *   if i is even  -> P(i) = m - 1 - i/2
 *   if i is odd   -> P(i) = m + (i - 1)/2
 * The answer a(n) is the least common multiple of the cycle lengths of P.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – behaviour below is irrelevant */
        return (seq_t)0;

    /* allocate visited array */
    bool *vis = (bool *)calloc((size_t)n, sizeof(bool));
    if (!vis)
        return (seq_t)0;

    /* generate all primes up to n (they are needed for factorisation) */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);
    if (!primes)
    {
        free(vis);
        return (seq_t)0;
    }

    /* array of maximal exponent per prime for LCM */
    size_t *max_exp = (size_t *)calloc(primes_cnt, sizeof(size_t));
    if (!max_exp)
    {
        free(vis);
        free(primes);
        return (seq_t)0;
    }

    const idx_t m = (n + 1) / 2;    /* number of even indices */

    /* traverse permutation cycles */
    for (idx_t i = 0; i < n; ++i)
    {
        if (vis[i])
            continue;

        idx_t cur = i;
        idx_t len = 0;
        do
        {
            vis[cur] = true;
            /* next position after one shuffle */
            if ((cur & 1ULL) == 0)      /* even index */
                cur = m - 1ULL - (cur >> 1);
            else                       /* odd index */
                cur = m + ((cur - 1ULL) >> 1);
            ++len;
        } while (cur != i);

        if (len <= 1)   /* cycle of length 1 does not affect LCM */
            continue;

        /* factorise len and update max exponents */
        idx_t rem = len;
        for (size_t pi = 0; pi < primes_cnt; ++pi)
        {
            uint64_t p = primes[pi];
            if ((idx_t)p * (idx_t)p > rem)
                break;

            size_t e = 0;
            while ((rem % (idx_t)p) == 0)
            {
                rem /= (idx_t)p;
                ++e;
            }
            if (e > max_exp[pi])
                max_exp[pi] = e;
        }
        if (rem > 1)   /* remaining prime factor */
        {
            /* rem is one of the primes in the list */
            for (size_t pi = 0; pi < primes_cnt; ++pi)
            {
                if (primes[pi] == (uint64_t)rem)
                {
                    if (max_exp[pi] < 1)
                        max_exp[pi] = 1;
                    break;
                }
            }
        }
    }

    /* compute lcm from prime powers */
    seq_t lcm = (seq_t)1;
    for (size_t pi = 0; pi < primes_cnt; ++pi)
    {
        if (max_exp[pi] > 0)
        {
            seq_t prime_sq = pow_seq((seq_t)primes[pi], (int)max_exp[pi]);
            lcm = seq_mul(lcm, prime_sq);
        }
    }

    /* clean up */
    free(vis);
    free(max_exp);
    free(primes);

    return lcm;
}
