/*
 * A220427  G.f.: exp( Sum_{n>=1} A005064(n)*x^n/n ), where A005064(n) = sum
 *          of cubes of primes dividing n.
 * https://oeis.org/A220427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in exp( sum_{k>=1} A005064(k) * x^k /
 *            k ) , where A005064(k) = sum of cubes of distinct prime divisors
 *            of k
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   compute_S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is a(0)=1; for n>=1,
   a(n) = 1/n * sum_{k=1..n} A005064(k) * a(n-k). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compute_S.h"

/* ---------- cached data ------------------------------------------- */
static seq_t *a_cache   = NULL;   /* a[n]      */
static seq_t *S_cache   = NULL;   /* A005064(n) */
static idx_t *spf_cache = NULL;   /* smallest prime factor table */
static idx_t  cap       = 0;      /* largest n that is cached */

/* ---------- small prime factor sieve ------------------------------- */
static void compute_spf(idx_t limit, idx_t *spf)
{
    spf[0] = spf[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* ---------- recurrence for a[n] ----------------------------------- */
static void compute_a(idx_t limit, const seq_t *S, seq_t *a)
{
    a[0] = (seq_t)1;
    for (idx_t n = 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k)
            sum += S[k] * a[n - k];
        a[n] = sum / (seq_t)n;
    }
}

/* ---------- ensure we have values up to n -------------------------- */
static void ensure_cap(idx_t n)
{
    if (n <= cap) return;

    /* Allocate new tables */
    seq_t  *new_a  = (seq_t  *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t  *new_S  = (seq_t  *)calloc((size_t)(n + 1), sizeof(seq_t));
    idx_t  *new_spf= (idx_t  *)calloc((size_t)(n + 1), sizeof(idx_t));

    if (!new_a || !new_S || !new_spf) {
        /* Allocation failure: keep old cache */
        if (new_a) free(new_a);
        if (new_S) free(new_S);
        if (new_spf) free(new_spf);
        return;
    }

    compute_spf(n, new_spf);
    compute_S(n, new_spf, new_S);
    compute_a(n, new_S, new_a);

    /* Free old tables */
    free(a_cache);
    free(S_cache);
    free(spf_cache);

    /* Store new tables */
    a_cache   = new_a;
    S_cache   = new_S;
    spf_cache = new_spf;
    cap       = n;
}

/* ---------- public interface -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 by definition */
    ensure_cap(n);
    return a_cache[(size_t)n];
}
