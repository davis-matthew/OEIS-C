/*
 * A079274  Number of divisors associated with the cyclic cases within the
 *          n-th group of least prime signatures.
 * https://oeis.org/A079274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A074141(n) - 2*A074141(n-1) for n>0 with a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 35, u64 n <= 75, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074141 is generated as the coefficient of 1/Product_{k>=1} (1-(k+1)x^k)   */
/* The recurrence is derived from the generating function */
#include <stdlib.h>

static seq_t *f_cache = NULL;
static size_t f_cache_cap = 0;

/* Compute coefficients f[0..N] where f satisfies 1/∏(1-(k+1)x^k) */
static void compute_f(size_t N)
{
    /* If we already have enough, nothing to do */
    if (N + 1 <= f_cache_cap)
        return;

    /* Allocate new cache */
    seq_t *new_cache = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!new_cache)
        return; /* failure can be handled by the caller */

    /* Initialize */
    for (size_t i = 0; i <= N; ++i)
        new_cache[i] = 0;
    new_cache[0] = 1;

    /* Dynamic programming over part sizes */
    for (size_t k = 1; k <= N; ++k) {
        seq_t mult = (seq_t)(k + 1);
        for (size_t i = k; i <= N; ++i) {
            new_cache[i] = new_cache[i] + mult * new_cache[i - k];
        }
    }

    /* Replace old cache */
    free(f_cache);
    f_cache = new_cache;
    f_cache_cap = N + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    size_t N = (size_t)n;
    compute_f(N);                /* ensure f[0..N] are available */

    /* a(n) = f[n] - 2*f[n-1] */
    return f_cache[N] - 2 * f_cache[N - 1];
}
