/*
 * A232853  Repeat n+1 times A091137(n).
 * https://oeis.org/A232853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, less
 * Formula:   a(n) = A091137(floor((√(8n+1)-1)/2)) A091137 sequence: a(0) = 1
 *            for k ≥ 1, if k is odd -> a(k) = 2 * a(k-1) else -> a(k) =
 *            a(k-1) * Π_{p prime, p-1 | k} p
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*--- Utility to compute integer row index from flattened index n ---*/
static idx_t row_index(idx_t n)
{
    long double f = (long double)8 * (long double)n + 1.0L;
    unsigned long long r = (unsigned long long)sqrtl(f);
    unsigned __int128 v = (unsigned __int128)8 * n + 1;
    while ((unsigned __int128)r * r > v)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 1) <= v)
        ++r;
    return (idx_t)((r - 1) / 2);
}

/*--- Cache for A091137 values ---*/
static seq_t *a_vals      = NULL;
static idx_t a_vals_len   = 0;

/*--- Cache for product of primes with (p-1) | n ---*/
static seq_t *factor      = NULL;
static idx_t factor_len   = 0;

/*--- Recompute factor array up to new_limit (inclusive) ---*/
static void factor_cache_update(idx_t new_limit)
{
    idx_t new_len = new_limit + 1;
    seq_t *new_factor = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
    for (idx_t i = 0; i < new_len; ++i) new_factor[i] = (seq_t)1;

    /* Generate primes up to new_limit+1 */
    size_t prime_limit = new_limit + 1;
    size_t sieve_size  = prime_limit + 1;
    char *is_prime = (char *)malloc(sieve_size);
    for (size_t i = 0; i < sieve_size; ++i) is_prime[i] = 1;
    if (sieve_size > 0) is_prime[0] = 0;
    if (sieve_size > 1) is_prime[1] = 0;
    for (size_t p = 2; p * p <= prime_limit; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= prime_limit; j += p)
                is_prime[j] = 0;

    for (size_t p = 2; p <= prime_limit; ++p)
        if (is_prime[p])
        {
            idx_t mod = (idx_t)(p - 1);
            for (idx_t m = mod; m <= new_limit; m += mod)
                new_factor[m] = new_factor[m] * (seq_t)p;
        }

    free(is_prime);

    free(factor);
    factor      = new_factor;
    factor_len  = new_len;
}

/*--- Ensure A091137 values up to index max_index are computed ---*/
static void ensure_a_up_to(idx_t max_index)
{
    if (max_index < a_vals_len)
        return;

    if (max_index >= factor_len)
        factor_cache_update(max_index);

    idx_t new_len = max_index + 1;
    seq_t *new_a = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
    if (a_vals_len > 0)
        memcpy(new_a, a_vals, (size_t)a_vals_len * sizeof(seq_t));

    for (idx_t i = a_vals_len; i <= max_index; ++i)
    {
        if (i == 0)
            new_a[0] = (seq_t)1;
        else if (i & 1ULL)                /* odd */
            new_a[i] = new_a[i - 1] * (seq_t)2;
        else                              /* even */
            new_a[i] = new_a[i - 1] * factor[i];
    }

    free(a_vals);
    a_vals      = new_a;
    a_vals_len  = new_len;
}

/*--- Public sequence function ---*/
seq_t a(idx_t n)
{
    idx_t r = row_index(n);
    ensure_a_up_to(r);
    return a_vals[r];
}
