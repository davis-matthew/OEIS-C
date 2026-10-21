/*
 * A058080  Numbers whose product of divisors exceeds their square.
 * https://oeis.org/A058080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058080 – Numbers whose product of divisors exceeds their square
    A(n) is the n‑th positive integer with at least five divisors.
    (The product of all divisors of m is m^(d(m)/2); this exceeds m²
    exactly when d(m) > 4.) */

#include <stdlib.h>
#include <stdint.h>

/* Global state (kept between calls) */
static idx_t   sieve_limit = 0;      /* highest value for which data are known */
static seq_t  *seq         = NULL;   /* array of terms */
static idx_t   seqLen      = 0;      /* number of terms stored */
static idx_t  *spf         = NULL;   /* smallest prime factor array   */
static idx_t  *divcnt      = NULL;   /* divisor count array            */
static idx_t  *primes      = NULL;   /* list of primes used in sieve   */
static size_t  primes_cap  = 0;      /* allocated capacity for primes */
static size_t  primes_cnt  = 0;      /* number of primes found        */

/* Recompute all tables up to new_limit (exclusive). */
static void recompute(idx_t new_limit)
{
    /* Allocate fresh auxiliary arrays */
    idx_t *new_spf    = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
    idx_t *new_divcnt = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
    idx_t *new_primes = (idx_t *)malloc(256 * sizeof(idx_t));
    if (new_spf == NULL || new_divcnt == NULL || new_primes == NULL)
        abort();                         /* out of memory */

    /* Initialize sieve data */
    for (idx_t i = 0; i <= new_limit; ++i)
        new_spf[i] = 0;
    primes_cap = 256;
    primes_cnt = 0;

    /* Linear sieve (computes spf and primes list) */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {           /* i is prime */
            new_spf[i] = i;
            if (primes_cnt == primes_cap) {      /* grow array */
                primes_cap <<= 1;
                new_primes = (idx_t *)realloc(new_primes,
                                             primes_cap * sizeof(idx_t));
                if (new_primes == NULL) abort();
            }
            new_primes[primes_cnt++] = i;
        }
        for (size_t k = 0; k < primes_cnt; ++k) {
            idx_t p = new_primes[k];
            if (i > new_limit / p) break;
            idx_t ip = i * p;
            new_spf[ip] = p;
            if (p == new_spf[i]) break;
        }
    }

    /* Compute divisor counts from spf data */
    new_divcnt[1] = 1;
    for (idx_t i = 2; i <= new_limit; ++i) {
        idx_t p   = new_spf[i];
        idx_t e   = 1;
        idx_t m   = i / p;
        while (m % p == 0) { m /= p; ++e; }
        new_divcnt[i] = new_divcnt[m] * (e + 1);
    }

    /* Build the term list (numbers with at least 5 divisors) */
    size_t term_cnt = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (new_divcnt[i] >= 5) ++term_cnt;

    seq_t *new_seq = (seq_t *)malloc(term_cnt * sizeof(seq_t));
    if (new_seq == NULL) abort();

    size_t pos = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (new_divcnt[i] >= 5)
            new_seq[pos++] = (seq_t)i;

    /* Replace old tables with the new ones */
    free(spf);     free(divcnt);   free(primes);   free(seq);
    spf     = new_spf;
    divcnt  = new_divcnt;
    primes  = new_primes;
    seq     = new_seq;
    seqLen  = term_cnt;
    sieve_limit = new_limit;
}

/* Return the n‑th term of A058080 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    /* Ensure we have enough terms precomputed */
    while (seqLen < n) {
        idx_t new_limit = (sieve_limit == 0) ? 100 : sieve_limit * 2;
        if (new_limit <= sieve_limit) new_limit = sieve_limit + 100;
        recompute(new_limit);
    }

    return seq[n - 1];
}
