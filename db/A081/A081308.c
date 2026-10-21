/*
 * A081308  Number of ways to write n as sum of a prime and an 3-smooth
 *          number.
 * https://oeis.org/A081308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ prime p < n | n – p is 3‑smooth }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3‑smooth numbers are of the form 2^i · 3^j   */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t sieve_limit = 0;
static bool *isComposite = NULL;  /* true if composite  */
static idx_t *primeList   = NULL;
static size_t primeCount   = 0;

static idx_t smooth_limit = 0;
static bool *isSmooth   = NULL;

/* Rebuild the prime sieve and list up to 'limit' (inclusive) */
static void recompute_sieve(idx_t limit)
{
    if (limit < 2) limit = 2;

    bool *newComp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!newComp) abort();

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!newComp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                newComp[j] = true;

    /* Count primes first */
    size_t pn = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!newComp[i]) ++pn;

    /* Allocate array for primes */
    idx_t *pr = (idx_t *)malloc(pn * sizeof(idx_t));
    if (!pr) abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!newComp[i]) pr[idx++] = i;

    /* Replace old data */
    free(isComposite);
    free(primeList);
    isComposite = newComp;
    primeList   = pr;
    primeCount  = pn;
    sieve_limit = limit;
}

/* Rebuild the 3‑smooth table up to 'limit' (inclusive) */
static void recompute_smooth(idx_t limit)
{
    bool *newSmooth = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!newSmooth) abort();

    for (idx_t p2 = 1; p2 <= limit; p2 <<= 1)
        for (idx_t p3 = 1; p3 <= limit; p3 *= 3)
        {
            idx_t prod = p2 * p3;
            if (prod > limit) break;
            newSmooth[prod] = true;
        }

    free(isSmooth);
    isSmooth   = newSmooth;
    smooth_limit = limit;
}

/* Ensure that data up to n‑1 is available */
static void ensure_up_to(idx_t n)
{
    idx_t need = (n > 1) ? (n - 1) : 1;
    if (need > sieve_limit || need > smooth_limit)
    {
        recompute_sieve(need);
        recompute_smooth(need);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_up_to(n);

    seq_t count = 0;
    for (size_t i = 0; i < primeCount; ++i)
    {
        idx_t p = primeList[i];
        if (p >= n) break;            /* primes must be < n   */
        idx_t rem = n - p;
        if (rem <= smooth_limit && isSmooth[rem])
            ++count;
    }
    return count;
}
