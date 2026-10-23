/*
 * A013929  Numbers that are not squarefree. Numbers that are divisible by a
 *          square greater than 1. The complement of A005117.
 * https://oeis.org/A013929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the n‑th positive integer that is NOT squarefree.
 *            Implementation uses the Möbius inversion formula: count(x) = #
 *            {k ≤ x : k is not squarefree} = x - Σ_{i=1}^{⌊√x⌋} μ(i) ⌊x / i²⌋
 *            where μ(i) is the Möbius function. The nth term is obtained by
 *            binary searching for the smallest x with count(x)...
 * Range:     u8 n <= 98, u16 n <= 8788, u32 n <= 8788, u64 n <= 8788,
 *            u128 n <= 8788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility: floor(sqrt(x)) for uint64 ---------- */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 p = (__int128)mid * mid;
        if (p <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ---------- Möbius table (lazy, linear sieve) ---------- */
static int8_t *mu_vals      = NULL; /* μ[0..mu_size] */
static size_t mu_size       = 0;

/* Recompute μ values up to n (inclusive).  Called only when needed. */
static void recompute_mu(size_t n)
{
    if (n <= mu_size) return;          /* already computed */
    mu_vals = (int8_t *)realloc(mu_vals, (n + 1) * sizeof(int8_t));

    size_t *is_composite = (size_t *)calloc(n + 1, sizeof(size_t));
    uint32_t *primes = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    mu_vals[0] = 0;  /* unused */
    mu_vals[1] = 1;

    for (size_t i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            mu_vals[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t prod = (uint64_t)i * primes[j];
            if (prod > n) break;
            is_composite[prod] = 1;
            if (i % primes[j] == 0) {
                mu_vals[prod] = 0;
                break;
            } else {
                mu_vals[prod] = -mu_vals[i];
            }
        }
    }

    free(is_composite);
    free(primes);
    mu_size = n;
}

/* ---------- Count numbers ≤ x that are NOT squarefree ---------- */
static inline idx_t count_non_squarefree(idx_t x)
{
    idx_t lim = floor_sqrt_u64(x);
    if (lim > mu_size) recompute_mu((size_t)lim);

    idx_t sum_pos = 0; /* Σ μ(i)=+1  terms of ⌊x/i²⌋ */
    idx_t sum_neg = 0; /* Σ μ(i)=-1 terms of ⌊x/i²⌋ */

    for (idx_t i = 1; i <= lim; ++i) {
        int8_t mu = mu_vals[i];
        if (mu == 0) continue;
        idx_t term = x / (i * i);
        if (mu > 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* cnt = x - sum_pos + sum_neg */
    __int128 cnt = (__int128)x - sum_pos + sum_neg;
    return (idx_t)cnt;
}

/* ---------- Sequence entry function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Rough upper bound; doubles until large enough */
    idx_t high = n * 3 + 10;
    if (high < 4) high = 4;

    while (count_non_squarefree(high) < n)
        high <<= 1;               /* double */

    idx_t low = 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_non_squarefree(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
