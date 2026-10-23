/*
 * A157846  Numbers which can be a mean of the divisors of some arithmetic
 *          number A003601(k).
 * https://oeis.org/A157846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A157846 – Numbers which can be a mean of the divisors of
 * an arithmetic number A003601(k).
 *
 * Implementation: Enumerate all k up to a fixed limit, compute
 * σ(k) and τ(k) via a smallest‑prime‑factor sieve.  If σ(k)/τ(k)
 * is integer, store the mean.  Sort, deduplicate, and cache the
 * resulting list.  The function a(idx_t n) returns the n‑th
 * value (1‑based indexing).  The algorithm is O(LIMIT log LIMIT)
 * for the precomputation, and O(1) for each query after that.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: dynamic array of seq_t values. */
static void push_seq(const seq_t *value, seq_t **array, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 256 : (*cap << 1);
        *array = (seq_t *)realloc(*array, newcap * sizeof(seq_t));
        if (!*array) abort();
        *cap = newcap;
    }
    (*array)[*len] = *value;
    (*len)++;
}

/* Comparator for qsort (works for all integer seq_t and arbitrary‑precision
 * seq_t in C++ as the operator< is defined). */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*b < *a) return 1;
    return 0;
}

/* Internal static cache of precomputed means. */
static seq_t *cached_vals = NULL;
static size_t cached_len  = 0;
static bool   cached_ready = false;

/* Precompute all means up to a limit sufficient for the first 10 000 terms. */
static void precompute(void)
{
    /* Choose a limit large enough to have >10 000 unique means. */
    const uint64_t LIMIT = 5000000ULL;  /* 5 million */

    /* Smallest‑prime‑factor sieve. */
    uint32_t *spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!spf) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == 0) spf[i] = i;   /* primes themselves */

    /* Temporary array for collected means. */
    seq_t *tmp = NULL;
    size_t tmp_len = 0, tmp_cap = 0;

    /* Enumerate all k and compute σ(k)/τ(k). */
    for (uint32_t k = 1; k <= (uint32_t)LIMIT; ++k) {
        uint64_t sigma = 1;
        uint64_t tau   = 1;
        uint32_t n = k;

        while (n > 1) {
            uint32_t p = spf[n];
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            /* Compute (p^(e+1)-1)/(p-1) safely. */
            uint64_t p_pow = 1;
            for (uint32_t i = 0; i <= e; ++i) p_pow *= p;
            uint64_t sigma_factor = (p_pow - 1) / (p - 1);
            sigma *= sigma_factor;
            tau   *= (e + 1);
        }

        if (sigma % tau == 0) {
            seq_t mean = (seq_t)(sigma / tau);
            push_seq(&mean, &tmp, &tmp_len, &tmp_cap);
        }
    }

    /* Sort the means. */
    qsort(tmp, tmp_len, sizeof(seq_t), seq_compare);

    /* Deduplicate. */
    size_t uniq_len = 0;
    for (size_t i = 0; i < tmp_len; ++i) {
        if (uniq_len == 0 || seq_compare(&tmp[i], &tmp[uniq_len - 1]) != 0) {
            tmp[uniq_len] = tmp[i];
            ++uniq_len;
        }
    }

    /* Store the unique sorted list in the static cache. */
    cached_vals = tmp;       /* tmp is now the cache */
    cached_len  = uniq_len;
    cached_ready = true;

    free(spf);
}

/* Public sequence generator. */
seq_t a(idx_t n)
{
    if (!cached_ready) precompute();

    if (n == 0) return (seq_t)0;
    if ((idx_t)n > cached_len) return (seq_t)0;  /* out of pre‑computed range */

    return cached_vals[n - 1];
}
