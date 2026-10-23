/*
 * A169436  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   mul_by_37, mul_by_703
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169436: Number of reduced words of length n in a Coxeter group
 * with 39 generators S_i and relations S_i^2 = (S_iS_j)^32 = I.
 *
 * The generating function is
 *   (1 + 2∑_{k=1}^{31}t^k + t^32) /
 *   (1 - 37∑_{k=1}^{31}t^k + 703t^32) .
 *
 * Hence the coefficients satisfy
 *
 *   a(0) = 1,
 *   a(n) = 37·∑_{i=1}^{min(31,n)} a(n-i) + 2      (1 ≤ n ≤ 31),
 *   a(32) = 37·∑_{i=1}^{31} a(32-i) + 1 - 703·a(0),
 *   a(n) = 37·∑_{i=1}^{31} a(n-i) - 703·a(n-32)   (n ≥ 33).
 *
 * The routine below computes a(n) for any n ≥ 0, memoizing each
 * intermediate value to support arbitrary call order.
 */

#include <stdlib.h>
#include "../utils/mul_by_37.h"
#include "../utils/mul_by_703.h"

static seq_t *cache = NULL;      /* array of computed terms   */
static idx_t  cache_size = 0;    /* number of stored terms    */

/* ensure that cache holds values 0 … n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;            /* already computed          */

    /* resize the cache */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (cache == NULL) ?
                       (seq_t *)malloc(new_size * sizeof(seq_t)) :
                       (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    if (new_cache == NULL) abort();        /* allocation failure        */
    cache       = new_cache;
    cache_size  = new_size;

    /* compute new entries */
    for (idx_t k = cache_size > 0 ? cache_size - 1 : 0; k <= n; ++k)
    {
        seq_t val;

        if (k == 0)                   /* basis                       */
        {
            val = (seq_t)1;
        }
        else
        {
            /* sum of the preceding up to 31 terms */
            seq_t sum = (seq_t)0;
            idx_t lim = (k < 31) ? k : 31;
            for (idx_t j = 1; j <= lim; ++j)
                sum += cache[k - j];

            if (k < 32)                /* (1 ≤ k ≤ 31)                */
            {
                val = mul_by_37(sum) + (seq_t)2;
            }
            else if (k == 32)          /* special second coefficient  */
            {
                val = mul_by_37(sum) + (seq_t)1
                      - mul_by_703(cache[0]);
            }
            else                       /* (k ≥ 33)                    */
            {
                val = mul_by_37(sum) - mul_by_703(cache[k - 32]);
            }
        }

        cache[k] = val;
    }

    cache_size = n + 1;   /* now valid indices are 0…n           */
}

/* public routine: a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
