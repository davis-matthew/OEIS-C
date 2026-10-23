/*
 * A016315  Expansion of g.f. 1/((1 - 2*x)*(1 - 7*x)*(1 - 12*x)).
 * https://oeis.org/A016315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 122*a(n-2) + 168*a(n-3) Initial values: a(0)
 *            = 1, a(1) = 21, a(2) = 319
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* The cache is large enough for n up to 10 000 as required. */
static seq_t cache[10001];
static idx_t cache_len = 0;
static bool cache_inited = false;

seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (!cache_inited) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)319;
        cache_len = 3;
        cache_inited = true;
    }

    /* If the requested term is already computed, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Compute terms up to n using the linear recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];

        /* Compute 21*a(n-1) + 168*a(n-3) - 122*a(n-2) */
        seq_t term1 = a_nm1 * (seq_t)21;
        seq_t term3 = a_nm3 * (seq_t)168;
        seq_t sum   = term1 + term3;
        seq_t term2 = a_nm2 * (seq_t)122;

        seq_t value;
        if (sum >= term2)
            value = sum - term2;
        else
            value = (seq_t)0;   /* should never happen for this sequence */

        cache[i] = value;
    }

    cache_len = n + 1;
    return cache[n];
}
