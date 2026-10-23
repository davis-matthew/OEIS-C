/*
 * A046112  a(n) is smallest integral radius of circle centered at (0,0)
 *          having 8n-4 lattice points on its circumference; a(n)/2 is
 *          smallest half-integral radius circle centered at (1/2,0) having
 *          4n-2 lattice points; a(n)/3 is smallest third-integral radius
 *          circle centered at (1/3,0) having 2n-1 lattice points.
 * https://oeis.org/A046112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer radius R of a circle centred at (0,0)
 *            that has exactly 8 n − 4 lattice points on its circumference
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t base, idx_t exp); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

/* 1‑mod‑4 primes – enough for the required factorizations */
static const idx_t primes1mod4[20] = {
    5, 13, 17, 29, 37, 41, 53, 61, 73,
    89, 97, 101, 109, 113, 137, 149, 157, 173,
    181, 193
};

/* depth‑first search over multiplicative partitions of M = 2n‑1 */
static void dfs(idx_t rem, idx_t start_f, int depth,
                int *exp_arr, seq_t *best, bool *found)
{
    if (rem == 1) {
        /* evaluate the candidate product */
        int exps[20];
        for (int i = 0; i < depth; ++i) exps[i] = exp_arr[i];

        /* sort exponents in non‑increasing order */
        for (int i = 0; i < depth - 1; ++i)
            for (int j = i + 1; j < depth; ++j)
                if (exps[i] < exps[j]) {
                    int tmp = exps[i]; exps[i] = exps[j]; exps[j] = tmp;
                }

        seq_t prod = 1;
        for (int i = 0; i < depth; ++i) {
            seq_t term = pow_seq((seq_t)primes1mod4[i], (idx_t)exps[i]);
            prod = seq_mul(prod, term);
        }

        if (!*found || prod < *best) {
            *best = prod;
            *found = true;
        }
        return;
    }

    /* try all odd divisors f of rem that are >= start_f */
    for (idx_t f = start_f; f <= rem; f += 2) {
        if (f == 1) continue; /* skip trivial factor */
        if (rem % f == 0) {
            exp_arr[depth] = (int)((f - 1) / 2);
            dfs(rem / f, f, depth + 1, exp_arr, best, found);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, safety */
    if (n == 1) return 1;      /* a(1) = 1 */

    idx_t M = 2 * n - 1;           /* odd product to decompose */
    int  exp_arr[20];
    seq_t best = 0;
    bool found = false;

    dfs(M, 3, 0, exp_arr, &best, &found);
    return best;
}
