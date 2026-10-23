/*
 * A193235  G.f.: A(x) = ( Sum_{n>=0} (-4)^n * x^(n^2) )^(-1/2).
 * https://oeis.org/A193235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies S(x) * A(x)^2 = 1, where S(x) = Sum_{k>=0} (-4)^k
 *            * x^{k^2}. For n>=1: 0 = sum_{k=0}^n s_k * c_{n-k}, with c_m =
 *            sum_{i=0}^m a_i * a_{m-i}. This gives the recurrence for a_n
 *            implemented below
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *A_cache = NULL;   /* a(n) */
static seq_t *C_cache = NULL;   /* c(n) = sum_{i=0}^n a_i * a_{n-i} */
static idx_t cache_size = 0;    /* allocated size */
static idx_t computed_max = (idx_t)(-1); /* highest n fully computed */

/* Make sure the caches can hold values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_cap = (size_t)(n + 1);
    A_cache = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    C_cache = (seq_t *)realloc(C_cache, new_cap * sizeof(seq_t));
    for (size_t i = cache_size; i < new_cap; ++i) {
        A_cache[i] = 0;
        C_cache[i] = 0;
    }
    cache_size = new_cap;

    if (computed_max == (idx_t)(-1)) {
        /* initialise a(0) and c(0) */
        A_cache[0] = 1;
        C_cache[0] = 1;   /* c(0) = a(0)*a(0) = 1 */
        computed_max = 0;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n <= computed_max) return A_cache[n];

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* ---- compute c(i) ---- */
        seq_t sum_c = 0;
        seq_t pow4 = (seq_t)(-4);   /* (-4)^1 */
        for (idx_t j = 1; (j * j) <= i; ++j) {
            idx_t idx = i - j * j;
            seq_t prod = pow4 * C_cache[idx];
            sum_c += prod;
            pow4 *= (seq_t)(-4);
        }
        C_cache[i] = -sum_c;

        /* ---- compute a(i) ---- */
        seq_t sum_prod = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum_prod += A_cache[k] * A_cache[i - k];
        }
        A_cache[i] = (C_cache[i] - sum_prod) / (seq_t)2;
    }

    computed_max = n;
    return A_cache[n];
}
