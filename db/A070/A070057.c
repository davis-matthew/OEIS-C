/*
 * A070057  Number of Bottleneck-Monge matrices with 9 rows.
 * https://oeis.org/A070057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(18,n)} P[k-1] * * S_{n-k} , where * P =
 *            {512,-9985,96161,-607903,2821517,-10164757,29260931,-68263237, *
 *            129682732,-200278047,249653465,-247990203,192418577,-113113161,
 *            * 48011519,-13562349,2175308,-131072} * * S_t = Σ_{i=0}^{t}
 *            binom(i+16,16) * 2^i * * (The sequence is obtain...
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 27, u128 n <= 74,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* coefficient polynomial of the generating function */
static const int32_t P[18] = {512, -9985, 96161, -607903, 2821517,
                              -10164757, 29260931, -68263237,
                              129682732, -200278047, 249653465,
                              -247990203, 192418577, -113113161,
                              48011519, -13562349, 2175308, -131072};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1, below is unused */

    /* we need S[0..n-1] */
    size_t size = (size_t)(n - 1);
    seq_t *S = seq_alloc_new_array(size + 1);

    seq_t pow2 = 1;      /* 2^i while iterating i */
    for (idx_t i = 0; i <= size; ++i) {
        /* binom(i+16,16) */
        seq_t bin = 1;
        for (idx_t j = 1; j <= 16; ++j) {
            bin = bin * (idx_t)(i + 16 - j + 1);   /* (i+17-j) */
            bin = bin / j;                         /* exact division */
        }
        seq_t bi = seq_mul(bin, pow2);            /* bin * 2^i */

        if (i == 0)
            S[0] = bi;
        else
            S[i] = S[i - 1] + bi;                  /* cumulative sum */

        pow2 = seq_mul(pow2, (seq_t)2);           /* next 2^(i+1) */
    }

    /* compute a(n) */
    seq_t result = 0;
    size_t maxk = n < 18 ? (size_t)n : 18;
    for (size_t k = 1; k <= maxk; ++k) {
        int32_t pk = P[k - 1];
        seq_t mult = seq_mul(S[n - k], (seq_t)(pk >= 0 ? pk : -pk));
        if (pk >= 0)
            result += mult;
        else
            result -= mult;
    }

    seq_free_array(S);
    return result;
}
