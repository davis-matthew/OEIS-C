/*
 * A163838  a(n) = (n-th composite) * (number of nontrivial divisors of n-th
 *          composite).
 * https://oeis.org/A163838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑th composite) * (τ(n‑th composite) – 2)
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ(m) is the number of divisors of m (including 1 and m) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute the n-th term of OEIS sequence A163838 */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Cache and sieve data: retained across calls
       ------------------------------------------------------------------ */
    static idx_t  sieve_limit = 0;         /* current upper bound of sieve */
    static uint64_t *spf = NULL;          /* smallest prime factor for each i */
    static uint64_t *tau = NULL;          /* divisor count τ(i) for each i */
    static idx_t  last_limit = 0;          /* last number processed for composites */

    /* Composite list: store composites and their divisor counts */
    static uint64_t *comps    = NULL;      /* n‑th composite numbers */
    static uint64_t *compdivs = NULL;      /* τ(composite) values */
    static size_t   comps_cnt = 0;         /* number of composites cached   */
    static size_t   comps_cap = 0;         /* current capacity of arrays     */

    /* ------------------------------------------------------------------
       Ensure we have at least n composites cached.  If not, extend the
       sieve and composite list.  The sieve is rebuilt from scratch each
       time it needs to grow; this is fast for n up to 10 000.
       ------------------------------------------------------------------ */
    while (comps_cnt < n)
    {
        /* Choose a new limit: double the current limit or at least one
           more than the last processed number.  Start with 16. */
        idx_t new_limit = (sieve_limit == 0) ? 16 : sieve_limit << 1;
        if (new_limit < last_limit + 1)
            new_limit = last_limit + 1;

        /* Allocate new spf and tau arrays up to new_limit */
        uint64_t *new_spf = (uint64_t *)malloc(sizeof(uint64_t) * (new_limit + 1));
        uint64_t *new_tau = (uint64_t *)malloc(sizeof(uint64_t) * (new_limit + 1));
        if (!new_spf || !new_tau) return (seq_t)0;           /* out of memory */

        /* Reset spf array */
        for (idx_t i = 0; i <= new_limit; ++i)
            new_spf[i] = 0;

        /* Compute smallest prime factor for each integer up to new_limit */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (new_spf[i] == 0) {          /* i is prime */
                new_spf[i] = i;
                if (i * i <= new_limit) {
                    for (idx_t j = i * i; j <= new_limit; j += i)
                        if (new_spf[j] == 0)
                            new_spf[j] = i;
                }
            }
        }

        /* Compute divisor counts τ(i) using the sieve */
        new_tau[1] = 1;
        for (idx_t i = 2; i <= new_limit; ++i) {
            uint64_t p = new_spf[i];
            idx_t t = i;
            int e = 0;
            while (t % p == 0) { ++e; t /= p; }
            new_tau[i] = new_tau[t] * (uint64_t)(e + 1);
        }

        /* Append new composite numbers to the cache */
        for (idx_t i = last_limit + 1; i <= new_limit; ++i) {
            if (i > 1 && new_spf[i] != i) {           /* composite */
                if (comps_cnt == comps_cap) {         /* expand capacity */
                    size_t new_cap = comps_cap ? comps_cap * 2 : 16;
                    uint64_t *tmp1 = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
                    uint64_t *tmp2 = (uint64_t *)realloc(compdivs, new_cap * sizeof(uint64_t));
                    if (!tmp1 || !tmp2) return (seq_t)0;  /* out of memory */
                    comps = tmp1;
                    compdivs = tmp2;
                    comps_cap = new_cap;
                }
                comps[comps_cnt] = (uint64_t)i;
                compdivs[comps_cnt] = new_tau[i];
                ++comps_cnt;
            }
        }

        /* Free old sieve data and replace with new ones */
        free(spf);
        free(tau);
        spf = new_spf;
        tau = new_tau;
        last_limit = new_limit;
        sieve_limit = new_limit;
    }

    /* ------------------------------------------------------------------
       Compute the final term: composite * (τ(composite) - 2)
       ------------------------------------------------------------------ */
    seq_t value = (seq_t)comps[n - 1];
    seq_t divcnt = (seq_t)compdivs[n - 1];
    return value * (divcnt - (seq_t)2);
}
