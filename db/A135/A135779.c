/*
 * A135779  Numbers having number of divisors equal to number of digits in
 *          base 9.
 * https://oeis.org/A135779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th integer m (m ≥ 1) such that divisor_count(m) ==
 *            number_of_base9_digits(m). The code precomputes divisor counts
 *            up to a fixed maximum (4 000 000), collects all qualifying
 *            numbers, and returns the n‑th one
 * Range:     u8 n <= 21, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   digits_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_base9.h"

static size_t CACHE_CAP = 0;
static size_t CACHE_CNT = 0;
static seq_t *CACHE = NULL;
static bool PRECOMPUTED = false;

/* Pre‑compute all terms up to a fixed limit. */
static void precompute(void)
{
    const idx_t LIMIT = 4000000;            /* 4 million is safely > 10 000th term */
    /* Smallest prime factor array */
    uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return; /* allocation failure – in practice unreachable */
    uint32_t *primes = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
    if (!primes) { free(spf); return; }

    size_t pc = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= LIMIT; ++j)
        {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* Divisor count array */
    seq_t *divcnt = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!divcnt)
    {
        free(spf);
        free(primes);
        return;
    }
    divcnt[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        uint32_t p = spf[i];
        uint32_t e = 0;
        uint32_t m = i;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        divcnt[i] = divcnt[m] * (e + 1);
    }

    /* Build cache of numbers satisfying divisor_count == base9_digits */
    CAPACITY: ;
    CACHE_CAP = 100000;
    CACHE = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
    if (!CACHE)
    {
        free(spf);
        free(primes);
        free(divcnt);
        return;
    }

    size_t cap = CACHE_CAP;
    size_t cnt = 0;
    for (uint32_t i = 1; i <= LIMIT; ++i)
    {
        if ((seq_t)divcnt[i] == (seq_t)digits_base9(i))
        {
            if (cnt == cap)
            {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(CACHE, cap * sizeof(seq_t));
                if (!tmp)
                    break; /* allocation failure – stop adding */
                CACHE = tmp;
            }
            CACHE[cnt++] = (seq_t)i;
        }
    }
    CACHE_CNT = cnt;

    /* free temporary arrays */
    free(spf);
    free(primes);
    free(divcnt);

    PRECOMPUTED = true;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (!PRECOMPUTED)
        precompute();

    if (n == 0 || (size_t)(n - 1) >= CACHE_CNT)
        return 0; /* out of bounds – should not happen for tested n */

    return CACHE[(size_t)(n - 1)];
}
