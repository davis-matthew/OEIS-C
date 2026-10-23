/*
 * A151555  G.f.: (1 + 2x) * Product_{n>=1} (1 + x^(2^n-1) + 2*x^(2^n)).
 * https://oeis.org/A151555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1 + 2x) * Product_{n>=1} (1 + x^(2^n-1) + 2*x^(2^n))
 * Range:     u8 n <= 60, u16 n <= 4086, u32 n <= 9283, u64 n <= 9283,
 *            u128 n <= 9283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void compute_upto(idx_t n_max)
{
    seq_t *new_cache = (seq_t *)malloc((n_max + 1) * sizeof(seq_t));
    if (!new_cache)
        return; /* if allocation fails, leave the old cache intact */

    for (idx_t i = 0; i <= n_max; ++i)
        new_cache[i] = 0;

    new_cache[0] = 1;
    if (n_max >= 1)
        new_cache[1] = 2;

    for (idx_t m = 1; ; ++m)
    {
        idx_t two_pow = ((idx_t)1) << m;
        idx_t exp1 = two_pow - 1;
        if (exp1 > n_max)
            break;
        idx_t exp2 = two_pow;

        for (idx_t k = n_max; ; --k)
        {
            seq_t sum = new_cache[k];
            if (k >= exp1)
                sum += new_cache[k - exp1];
            if (k >= exp2)
                sum += new_cache[k - exp2] * 2;
            new_cache[k] = sum;
            if (k == 0)
                break;
        }
    }

    if (cache)
        free(cache);
    cache = new_cache;
    cache_size = n_max + 1;
}

seq_t a(idx_t n)
{
    if (!cache || n >= cache_size)
        compute_upto(n);
    return cache[n];
}
