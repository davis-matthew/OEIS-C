/*
 * A061200  tau_5(n) = number of ordered 5-factorizations of n.
 * https://oeis.org/A061200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = tau_5(n) – number of ordered 5‑factorizations of n. tau_5
 *            is multiplicative and tau_5(p^m) = C(m+4,4)
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple integer square root: floor(sqrt(n))                          */
/* ------------------------------------------------------------------ */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Sieve to generate all primes ≤ limit.  The primes are returned in   */
/* an array sorted ascendingly.  The caller is responsible for freeing */
/* the returned array.                                                 */
/* ------------------------------------------------------------------ */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t size = (size_t)(limit + 1);
    bool *is_composite = (bool *)calloc(size, sizeof(bool));
    size_t prime_count = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[(size_t)j] = true;
        }
    }

    /* Count primes. */
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i]) ++prime_count;

    uint64_t *primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i]) primes[idx++] = (uint64_t)i;

    free(is_composite);
    *primes_out = primes;
    *cnt_out = prime_count;
}

/* ------------------------------------------------------------------ */
/* Static storage for a growing prime table.                           */
/* ------------------------------------------------------------------ */
static uint64_t *prime_table   = NULL;
static size_t   prime_cnt      = 0;
static idx_t    prime_limit    = 0;

/* Ensure primes up to 'need'.  Expands the table if necessary.       */
static void ensure_primes(idx_t need)
{
    if (prime_limit >= need)
        return;

    /* free old */
    if (prime_table)
        free(prime_table);

    build_primes_upto(need, &prime_table, &prime_cnt);
    prime_limit = need;
}

/* ------------------------------------------------------------------ */
/* Binomial coefficient C(e+4,4) computed multiplicatively.           */
/* ------------------------------------------------------------------ */
static seq_t binom4(uint64_t e)
{
    seq_t r = (seq_t)(e + 4);
    r *= (seq_t)(e + 3);
    r *= (seq_t)(e + 2);
    r *= (seq_t)(e + 1);
    r /= (seq_t)24;   /* 4! = 24 */
    return r;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;        /* tau_5(1) = 1 */

    uint64_t m       = (uint64_t)n;
    seq_t    result  = (seq_t)1;

    /* Need primes up to sqrt(n). */
    idx_t limit = isqrt(n);
    if (limit < 2)
        limit = 2;
    ensure_primes(limit);

    /* Trial division by primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_table[i];
        if ((unsigned __int128)p * p > m)
            break;

        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= binom4(exp);
        }
    }

    /* Remaining prime factor (if any) has exponent 1. */
    if (m > 1)
        result *= binom4(1);

    return result;
}
