/*
 * A136092  Bisection of A138540.
 * https://oeis.org/A136092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2n-1)*(5n^2+22n+15)*a(n-1) - *
 *            36*(n-1)*(2n-1)*(2n-3)*a(n-2) divided by * (n+3)*(n+5)*(n+6);
 *            a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len) return cache[n];

    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0 || k == 1) {
            /* already cached */
            continue;
        }

        seq_t a_nm1 = cache[k-1];
        seq_t a_nm2 = cache[k-2];

        __uint128_t n128   = (__uint128_t)k;
        __uint128_t two_n_minus_1 = 2*n128 - 1;
        __uint128_t five_n_sq_plus = 5*n128*n128 + 22*n128 + 15;
        __uint128_t n_minus_1 = n128 - 1;
        __uint128_t two_n_minus_3 = 2*n128 - 3;

        __uint128_t term1 = 4 * two_n_minus_1 * five_n_sq_plus * ((__uint128_t)a_nm1);
        __uint128_t term2 = 36 * n_minus_1 * two_n_minus_1 * two_n_minus_3 * ((__uint128_t)a_nm2);
        __uint128_t numerator = term1 - term2;
        __uint128_t denom = (n128 + 3) * (n128 + 5) * (n128 + 6);

        cache[k] = (seq_t)(numerator / denom);
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
