/*
 * A054799  Integers n such that sigma(n+2) = sigma(n) + 2, where sigma =
 *          A000203, the sum of divisors of n.
 * https://oeis.org/A054799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based generation of A054799:
   a(n) = the n-th integer m such that σ(m+2) = σ(m) + 2
   (σ = sum of divisors).  The sequence is pre‑computed up to a fixed
   upper bound and the first *n* values are returned from the cache. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Parameters – two‑million is enough for the first 10 000 terms.     */
/* Adjust LIMIT if a larger domain is required.                       */
/* ------------------------------------------------------------------ */
#define LIMIT 2000000
#define MAX_TERMS 20000

/* ------------------------------------------------------------------ */
/* Global state – these tables are filled once on the first call.     */
/* ------------------------------------------------------------------ */
static seq_t sigma[LIMIT + 5];
static idx_t spf[LIMIT + 5];      /* smallest prime factor */
static seq_t terms[MAX_TERMS];
static size_t terms_cnt = 0;
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Linear sieve to compute smallest prime factors up to LIMIT.        */
/* ------------------------------------------------------------------ */
static void init_spf(void)
{
    size_t i, j;
    size_t primes_cnt = 0;
    static idx_t primes[150000];   /* enough for LIMIT */

    for (i = 0; i <= (size_t)LIMIT; ++i) spf[i] = 0;

    for (i = 2; i <= (size_t)LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (idx_t)i;
            primes[primes_cnt++] = (idx_t)i;
        }
        for (j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || (idx_t)i * p > (idx_t)LIMIT) break;
            spf[(idx_t)i * p] = p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute σ(n) for all 1 ≤ n ≤ LIMIT using the spf table.            */
/* ------------------------------------------------------------------ */
static void compute_sigma(void)
{
    size_t i, k;
    sigma[0] = (seq_t)0;
    sigma[1] = (seq_t)1;
    for (i = 2; i <= (size_t)LIMIT; ++i) {
        seq_t res = (seq_t)1;
        idx_t m = (idx_t)i;
        while (m > 1) {
            idx_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* sum_{j=0..e} p^j */
            seq_t term = (seq_t)1;
            seq_t p_pow = (seq_t)1;
            for (k = 0; k < e; ++k) {
                p_pow = p_pow * (seq_t)p;
                term  = term + p_pow;
            }
            res = res * term;
        }
        sigma[i] = res;
    }
}

/* ------------------------------------------------------------------ */
/* Fill the terms array with the first MAX_TERMS values of the         */
/* sequence.  This is done lazily on the first request.                */
/* ------------------------------------------------------------------ */
static void prepare(void)
{
    if (initialized) return;
    init_spf();
    compute_sigma();

    size_t i;
    for (i = 1; i <= (size_t)LIMIT - 2; ++i) {
        if (sigma[i + 2] == sigma[i] + (seq_t)2) {
            if (terms_cnt < MAX_TERMS)
                terms[terms_cnt++] = (seq_t)i;
            else
                break;
        }
    }
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public entry point – returns the n-th term (offset 1).             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    prepare();

    if ((size_t)n <= terms_cnt)
        return terms[(size_t)n - 1];
    /* If the requested index exceeds the precomputed range,
       return 0.  The problem limits n to 10 000, well below the
       precomputed capacity. */
    return (seq_t)0;
}
