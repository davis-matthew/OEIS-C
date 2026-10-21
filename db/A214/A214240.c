/*
 * A214240  Number of 0..7 colorings of a 2X(n+1) array circular in the n+1
 *          direction with new values 0..7 introduced in row major order.
 * https://oeis.org/A214240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 0..7‑colorings of a 2×(n+1) array cyclic
 *            in the n+1 direction with new values introduced in row‑major
 *            order. The sequence is defined by the linear recurrence (order
 *            12): a(1) = 4 a(2) = 18 a(3) = 354 a(4) = 7749 a(5) = 231799
 *            a(6) = 8178855 a(7) = 318511707 a(8) = 1304674848...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Dynamic array growth helper used only inside this file. */
static void grow_cache(seq_t **p, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*p, sizeof(seq_t) * new_cap);
    if (!tmp) /* out of memory – abort */
        abort();
    *p   = tmp;
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1 – return 0 for safety */
        return (seq_t)0;

    /* Initial 12 terms (stored 0‑based in the array). */
    static const seq_t init[12] = {
        (seq_t)4ULL,
        (seq_t)18ULL,
        (seq_t)354ULL,
        (seq_t)7749ULL,
        (seq_t)231799ULL,
        (seq_t)8178855ULL,
        (seq_t)318511707ULL,
        (seq_t)13046748480ULL,
        (seq_t)547969772934ULL,
        (seq_t)23296216625352ULL,
        (seq_t)996235422019600ULL,
        (seq_t)42723792981743991ULL
    };

    /* Recurrence coefficients. */
    static const int coeff[12] = {
          66,   -867,   -8354,    120414,   672756,
        -3902534, -24478884,  5777547,
         189931178, 205081401, -166116762, -207075960
    };

    /* Absolute values of coefficients for unsigned multiplication. */
    static const unsigned long long abs_coeff[12] = {
          66ULL,   867ULL,   8354ULL,    120414ULL,   672756ULL,
        3902534ULL, 24478884ULL, 5777547ULL,
        189931178ULL, 205081401ULL, 166116762ULL, 207075960ULL
    };

    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;     /* allocated elements */
    static idx_t  cache_max = 0;     /* highest n with cache[n-1] valid */

    /* First call – initialise cache with the first 12 terms. */
    if (!cache) {
        grow_cache(&cache, &cache_cap, 12);
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_max = 12;
    }

    /* Return already computed value if possible. */
    if (n <= cache_max)
        return cache[n - 1];

    /* Ensure the cache can hold terms up to n. */
    grow_cache(&cache, &cache_cap, (size_t)n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 12; ++k) {
            /* a(i‑1) … a(i‑12) are at indices i‑2 … i‑13 in the array. */
            seq_t prod = cache[i - k - 2] * (seq_t)abs_coeff[k];
            if (coeff[k] >= 0)
                pos_sum += prod;
            else
                neg_sum += prod;
        }
        cache[i - 1] = pos_sum - neg_sum;
    }

    cache_max = n;
    return cache[n - 1];
}
