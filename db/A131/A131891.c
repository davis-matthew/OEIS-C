/*
 * A131891  a(n) is the number of shapes of balanced trees with constant
 *          branching factor 5 and n nodes. The node is balanced if the size,
 *          measured in nodes, of each pair of its children differ by at most
 *          one node.
 * https://oeis.org/A131891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = a(1) = 1; For n >= 2, write n = 5k + 1 + m, 0 <= m <= 4,
 *            where k = floor(n/5) and m = (n % 5 == 0) ? 4 : (n % 5) - 1.
 *            Then a(n) = binom(5, m) * a(k+1)^m * a(k)^(5-m) (all operations
 *            in seq_t)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 37, u64 n <= 57, u128 n <= 182,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = 0;

static const seq_t bin5[6] = {
    (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)10, (seq_t)5, (seq_t)1
};

/* Ensure the cache array is large enough to hold indices up to `need` */
static void ensure_cache_size(idx_t need)
{
    if (cache_size > (size_t)need)
        return;

    size_t new_size = cache_size > 0 ? cache_size : 2;
    while (new_size <= (size_t)need)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }

    /* initialize new entries (will be overwritten as needed)  */
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)1;   /* harmless default */

    cache      = new_cache;
    cache_size = new_size;
}

/* Compute all terms up to index `n`, store them in the cache */
static void compute_up_to(idx_t n)
{
    if (n <= computed_max)
        return;

    ensure_cache_size(n);

    /* initialise base terms if not yet done */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    if (computed_max < 1)
        computed_max = 1;

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 0 || i == 1) { /* already set */
            continue;
        }

        idx_t k  = i / 5;
        idx_t rem = i % 5;
        idx_t kk = k;
        idx_t m;

        if (rem == 0) {          /* use k' = k-1 and m = 4 */
            kk = k - 1;
            m = 4;
        } else {                 /* m = rem - 1 */
            m = rem - 1;
        }

        seq_t left  = cache[kk + 1]; /* a(k+1) */
        seq_t right = cache[kk];     /* a(k)   */

        seq_t pow_left  = seq_pow(left,  (idx_t)m);
        seq_t pow_right = seq_pow(right, (idx_t)(5 - m));
        seq_t prod      = seq_mul(pow_left, pow_right);
        seq_t coeff     = bin5[m];

        cache[i] = seq_mul(coeff, prod);
    }

    computed_max = n;
}

seq_t a(idx_t n)
{
    if (!cache)
        compute_up_to(1);          /* ensure cache exists with base terms */

    compute_up_to(n);
    return cache[n];
}
