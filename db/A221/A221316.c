/*
 * A221316  Number of nonnegative integer arrays of length n summing to n
 *          without equal adjacent values modulo 3.
 * https://oeis.org/A221316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{residue sequences r_1…r_n with no equal adjacent} ( n
 *            − Σ r_i )⁄3 is an integer × C( (n−Σ r_i)/3 + n−1 , n−1 )
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
 
/* multiplicative binomial coefficient */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}
 
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of range, not used */
 
    /* maximum possible sum of residues is 2*n (each residue ≤ 2) */
    const idx_t maxS = 2 * n;
    const size_t len = (size_t)maxS + 1;
 
    /* allocate arrays for previous and next DP layers */
    seq_t *cur[3], *nxt[3];
    for (int r = 0; r < 3; ++r)
    {
        cur[r] = (seq_t *)calloc(len, sizeof(seq_t));
        nxt[r] = (seq_t *)calloc(len, sizeof(seq_t));
        if (!cur[r] || !nxt[r])
        {
            /* memory allocation failure – clean up and return 0 */
            for (int rr = 0; rr <= r; ++rr)
            {
                if (cur[rr]) free(cur[rr]);
                if (nxt[rr]) free(nxt[rr]);
            }
            return 0;
        }
    }
 
    /* base case: length 1, residue equals value */
    cur[0][0] = 1;          /* r = 0 → sum 0 */
    cur[1][1] = 1;          /* r = 1 → sum 1 */
    cur[2][2] = 1;          /* r = 2 → sum 2 */
 
    /* iterate over positions 2 … n */
    for (idx_t pos = 2; pos <= n; ++pos)
    {
        /* clear next layer */
        for (int r = 0; r < 3; ++r)
            memset(nxt[r], 0, len * sizeof(seq_t));
 
        idx_t prevMax = 2 * (pos - 1);    /* maximum residue sum of previous position */
 
        for (int r = 0; r < 3; ++r)
        {
            int p1 = (r + 1) % 3;
            int p2 = (r + 2) % 3;
 
            /* contributions from residue p1 */
            for (idx_t s = 0; s <= prevMax; ++s)
            {
                seq_t v = cur[p1][s];
                if (v)
                    nxt[r][s + r] += v;
            }
            /* contributions from residue p2 */
            for (idx_t s = 0; s <= prevMax; ++s)
            {
                seq_t v = cur[p2][s];
                if (v)
                    nxt[r][s + r] += v;
            }
        }
 
        /* swap cur and nxt for next iteration */
        for (int r = 0; r < 3; ++r)
        {
            seq_t *tmp = cur[r];
            cur[r] = nxt[r];
            nxt[r] = tmp;
        }
    }
 
    /* compute final answer: sum over all sums s ≤ n with (n−s)%3==0 */
    seq_t ans = 0;
    for (idx_t s = 0; s <= n; ++s)
    {
        seq_t tot = cur[0][s] + cur[1][s] + cur[2][s];
        if (!tot)
            continue;
        idx_t diff = n - s;
        if (diff % 3 != 0)
            continue;
        idx_t t = diff / 3;                     /* t = (n−s)/3 */
        seq_t comb = binom_seq(t + (n - 1), n - 1);
        ans += comb * tot;
    }
 
    /* free allocated memory */
    for (int r = 0; r < 3; ++r)
    {
        free(cur[r]);
        free(nxt[r]);   /* nxt now holds old cur due to swap */
    }
 
    return ans;
}
