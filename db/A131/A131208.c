/*
 * A131208  Greatest prime divisor of all composite numbers between n-th
 *          semiprime and next semiprime, or 1 if there are no such composite
 *          numbers.
 * https://oeis.org/A131208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = MAX{ A006530(k) : A001358(n) < k < A001358(n+1) }
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of helper */
static void ensure_precomputation(idx_t n);

/* Static tables for sieve and semiprimes */
static bool initialized = false;
static uint32_t *spf = NULL;    /* smallest prime factor table */
static size_t  spf_limit = 0;   /* current sieve upper bound */
static seq_t  *semis = NULL;    /* first semiprimes */
static size_t  semis_len = 0;   /* number stored */

/* ------------------------------------------------------------------ */
/* ensure_precomputation: make sure we have at least n+1 semiprimes   */
/* ------------------------------------------------------------------ */
static void ensure_precomputation(idx_t n)
{
    /* Need semis[n] (i.e. n+1 semiprimes) */
    size_t required = (size_t)n + 1;
    if (initialized && semis_len > required)
        return;

    size_t limit = spf_limit ? spf_limit : 200000;   /* initial guess */

    while (true) {
        /* free previous tables */
        if (spf)   free(spf);
        if (semis) free(semis);

        /* allocate and build smallest‑prime‑factor table */
        spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
        for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }

        /* collect semiprimes up to required count (+ margin) */
        size_t max_need = required + 20;
        semis = (seq_t*)malloc(max_need * sizeof(seq_t));
        size_t cnt = 0;
        for (size_t m = 4; m <= limit && cnt < max_need; ++m) {
            uint32_t p = spf[m];
            uint32_t q = m / p;
            if (q >= 2 && spf[q] == q) {
                semis[cnt++] = (seq_t)m;
            }
        }
        semis_len = cnt;

        if (semis_len > required) {
            initialized = true;
            spf_limit = limit;
            break;
        }
        /* Need larger range: double the limit */
        limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/* a(n) : sequence A131208                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined below offset; keep safe return 0 */
        return (seq_t)0;

    ensure_precomputation(n);

    seq_t left  = semis[n - 1];
    seq_t right = semis[n];

    /* No composites in the interval?  Return 1. */
    if (left + 1 >= right)
        return (seq_t)1;

    uint32_t best = 0;
    for (uint64_t k = (uint64_t)left + 1; k < (uint64_t)right; ++k) {
        uint32_t spf_k = spf[(size_t)k];
        if (spf_k == (uint32_t)k)
            continue;                /* skip primes */

        /* find largest prime factor of k */
        uint32_t max_p = 0;
        uint32_t temp = (uint32_t)k;
        while (temp > 1) {
            uint32_t p = spf[temp];
            if (p > max_p) max_p = p;
            while (temp % p == 0) temp /= p;
        }
        if (max_p > best) best = max_p;
    }

    return best ? (seq_t)best : (seq_t)1;
}
