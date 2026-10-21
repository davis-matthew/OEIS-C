/*
 * A236445  Number of (n+1)X(1+1) 0..3 arrays colored with the difference of
 *          the maximum and minimum in each 2X2 subblock.
 * https://oeis.org/A236445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236445: Number of (n+1)×2 0..3 arrays colored with the
 * difference of the maximum and minimum in each 2×2 subblock.
 *
 * Empirical recurrence (valid for n > 13):
 *   a(n) = 32*a(n-2) + 90*a(n-3) + 846*a(n-4) + 2418*a(n-5)
 *        + 3809*a(n-6) + 5178*a(n-7) + 3384*a(n-8) + 3328*a(n-9)
 *        + 1024*a(n-10) + 512*a(n-11)
 *
 * Initial values (for 1 ≤ n ≤ 13):
 *   a(1)=256,   a(2)=2116,   a(3)=17258,   a(4)=136208,
 *   a(5)=1080984, a(6)=8564392, a(7)=67897642, a(8)=538065184,
 *   a(9)=4265291002, a(10)=33804568568, a(11)=267953875076,
 *   a(12)=2123741832432, a(13)=16833511636310
 *
 * The function below memoises computed values and works for any
 * seq_t / idx_t combination that satisfies the build constraints.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache: 1‑based indices, index 0 unused. */
static seq_t *cache          = NULL;
static idx_t cache_size      = 0;
static idx_t last_computed   = 0;

/* Initial terms a(1) … a(13).  Cast to seq_t for consistency. */
static const seq_t init_vals[13] = {
    (seq_t)256, (seq_t)2116, (seq_t)17258,
    (seq_t)136208, (seq_t)1080984, (seq_t)8564392,
    (seq_t)67897642, (seq_t)538065184, (seq_t)4265291002,
    (seq_t)33804568568, (seq_t)267953875076,
    (seq_t)2123741832432, (seq_t)16833511636310
};

/* Coefficients for the recurrence: c2 … c11. */
static const idx_t coeffs[10] = {
    32, 90, 846, 2418, 3809,
    5178, 3384, 3328, 1024, 512
};

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence starts at index 1 */

    /* Allocate or grow the cache to accommodate index n. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;            /* +1 for 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);     /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;          /* initialise new slots */
        cache_size = new_size;
    }

    /* If the requested term is already computed, return it. */
    if (n <= last_computed) return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t sum = 0;
            for (int k = 0; k < 10; ++k) {
                seq_t prod = cache[i - (2 + k)] * (seq_t)coeffs[k];
                sum += prod;
            }
            cache[i] = sum;
        }
    }

    last_computed = n;
    return cache[n];
}
