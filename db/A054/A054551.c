/*
 * A054551  Prime number spiral (clockwise, North spoke).
 * https://oeis.org/A054551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the (4*n^2 - 3*n + 1)-th prime number (1‑based index into
 *            the list of all primes)
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_prime_upper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are supplied by the build; no redefinition here. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_prime_upper.h"

/* ---------- Prime storage ----------
 * primes[0] = 2, primes[1] = 3, ...   uint64_t for storage
 */
static uint64_t *primes      = NULL;
static size_t   primes_cap   = 0;
static size_t   primes_cnt   = 0;

/* ---------- Small primes for sieving ----------
 * List of primes <= current limit, used to mark composites.
 */
static uint32_t *small_primes   = NULL;
static size_t    small_cap      = 0;
static size_t    small_cnt      = 0;
static uint32_t  small_last_lim = 0;

/* ---------- Append prime to dynamic array ----------
 */
static void append_prime(uint64_t val)
{
    if (primes_cnt == primes_cap) {
        primes_cap = primes_cap ? (primes_cap << 1) : 16;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    primes[primes_cnt++] = val;
}

/* ---------- Append small prime ----------
 */
static void append_small_prime(uint32_t val)
{
    if (small_cnt == small_cap) {
        small_cap = small_cap ? (small_cap << 1) : 16;
        small_primes = (uint32_t *)realloc(small_primes, small_cap * sizeof(uint32_t));
    }
    small_primes[small_cnt++] = val;
}

/* ---------- Ensure small primes up to 'lim' ----------
 */
static void ensure_small_primes(uint32_t lim)
{
    if (lim <= small_last_lim) return;

    uint32_t sz = lim + 1;
    uint8_t *is_comp = (uint8_t *)calloc(sz, 1);
    for (uint32_t i = 2; (uint64_t)i * i <= lim; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= lim; j += i)
                is_comp[j] = 1;
        }
    }

    free(small_primes);
    small_primes = NULL;
    small_cap    = 0;
    small_cnt    = 0;
    for (uint32_t i = 2; i <= lim; ++i) {
        if (!is_comp[i]) append_small_prime(i);
    }

    free(is_comp);
    small_last_lim = lim;
}

/* ---------- Find the k-th prime ----------
 * k is 1‑based; returns the prime value as seq_t.
 */
static seq_t find_kth_prime(uint64_t k)
{
    if (primes_cnt >= k) return (seq_t)primes[k - 1];

    /* Generate primes up to the needed upper bound. */
    uint64_t upper = estimate_prime_upper(k);
    const uint64_t seg_size = 1000000ULL;
    uint64_t start = 2;
    if (primes_cnt)
        start = primes[primes_cnt - 1] + 1;

    while (start <= upper) {
        uint64_t end = start + seg_size - 1;
        if (end > upper) end = upper;
        if (start > end) break;

        uint32_t sqrt_end = (uint32_t)(sqrt((double)end) + 1.0);
        ensure_small_primes(sqrt_end);

        uint32_t block_sz = (uint32_t)(end - start + 1);
        uint8_t *mark = (uint8_t *)calloc(block_sz, 1);

        for (size_t idx = 0; idx < small_cnt; ++idx) {
            uint64_t p = small_primes[idx];
            if (p * p > end) break;

            uint64_t first = (start + p - 1) / p * p;
            if (first < p * p) first = p * p;

            for (uint64_t j = first; j <= end; j += p)
                mark[j - start] = 1;
        }

        if (start <= 1 && 1 <= end) mark[1 - start] = 1;

        for (uint64_t i = start; i <= end; ++i) {
            if (!mark[i - start]) {
                append_prime(i);
                if (primes_cnt >= k) {
                    free(mark);
                    return (seq_t)primes[k - 1];
                }
            }
        }

        free(mark);
        start = end + 1;
    }

    /* In the unlikely case the estimate was too low, extend the search. */
    while (primes_cnt < k) {
        uint64_t new_upper = upper * 2 + 1000;
        start = upper + 1;
        upper = new_upper;
        const uint64_t seg_size2 = 1000000ULL;
        while (start <= upper) {
            uint64_t end = start + seg_size2 - 1;
            if (end > upper) end = upper;

            uint32_t sqrt_end = (uint32_t)(sqrt((double)end) + 1.0);
            ensure_small_primes(sqrt_end);

            uint32_t block_sz = (uint32_t)(end - start + 1);
            uint8_t *mark = (uint8_t *)calloc(block_sz, 1);

            for (size_t idx = 0; idx < small_cnt; ++idx) {
                uint64_t p = small_primes[idx];
                if (p * p > end) break;

                uint64_t first = (start + p - 1) / p * p;
                if (first < p * p) first = p * p;

                for (uint64_t j = first; j <= end; j += p)
                    mark[j - start] = 1;
            }

            if (start <= 1 && 1 <= end) mark[1 - start] = 1;

            for (uint64_t i = start; i <= end; ++i) {
                if (!mark[i - start]) {
                    append_prime(i);
                    if (primes_cnt >= k) {
                        free(mark);
                        return (seq_t)primes[k - 1];
                    }
                }
            }

            free(mark);
            start = end + 1;
        }
    }

    /* Should never happen. */
    return 0;
}

/* ---------- Main sequence function ----------
 */
seq_t a(idx_t n)
{
    /* compute k = 4*n*n - 3*n + 1 (k is 1‑based index of prime) */
    unsigned __int128 t = (unsigned __int128)4 * n * n
                       - (unsigned __int128)3 * n
                       + (unsigned __int128)1;
    uint64_t k = (uint64_t)t;   /* fits in 64 bits for all reasonable n */

    return find_kth_prime(k);
}
