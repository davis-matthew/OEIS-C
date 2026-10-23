/*
 * A145511  Dirichlet g.f.: (1-2/2^s+7/4^s)*zeta(s)^3.
 * https://oeis.org/A145511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over the prime‐power factorisation of n. If n =
 *            2^e * Π_{p>2} p^{e_p} then a(2^e) = 3*(e-1)*e + 1, a(p^{e_p}) =
 *            (e_p+1)*(e_p+2)/2 for odd primes p. The sequence is
 *            multiplicative
 * Range:     u8 n <= 191, u16 n <= 9646, u32 n <= 9646, u64 n <= 9646,
 *            u128 n <= 9646, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Ensure that the cache and the smallest‑prime‑factor table contain
   values up to at least 'n'.  All tables index from 0 to 'len'.  */
static void build_spf(uint64_t limit, uint64_t *arr)
{
    for (uint64_t i = 0; i <= limit; ++i) arr[i] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i)
    {
        if (arr[i] == 0)                    /* i is prime */
        {
            for (uint64_t j = i * i; j <= limit; j += i)
                if (arr[j] == 0) arr[j] = i;
        }
    }
    for (uint64_t i = 2; i <= limit; ++i)    /* fill remaining primes */
        if (arr[i] == 0) arr[i] = i;
}

static seq_t *seq_cache = NULL;     /* a(n) values, index from 0 */
static size_t cache_cap = 0;        /* highest index stored + 1 */
static uint64_t *spf_tbl = NULL;    /* smallest‑prime‑factor table */
static size_t spf_cap = 0;

/* Grow the cache and spf tables so that index n is available.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;

    size_t new_cap = (cache_cap == 0) ? (size_t)n + 1 : cache_cap * 2;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

    /* Allocate and build new tables.  */
    uint64_t *new_spf = (uint64_t *)malloc((new_cap + 1) * sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    build_spf(new_cap, new_spf);

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache) { free(new_spf); exit(EXIT_FAILURE); }

    /* Copy old cache values if any.  */
    if (seq_cache)
    {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = seq_cache[i];
    }

    /* Initialise base cases.  */
    new_cache[0] = (seq_t)0;            /* unused */
    if (new_cap >= 1) new_cache[1] = (seq_t)1;

    /* Compute new entries starting from first uncomputed index.  */
    size_t start = (cache_cap == 0) ? 2 : cache_cap;
    for (size_t i = start; i <= new_cap; ++i)
    {
        uint64_t m = i;
        uint64_t p = new_spf[m];
        uint64_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        seq_t em = (seq_t)e;
        seq_t f;
        if (p == 2)
        {
            /* a(2^e) = 3*(e-1)*e + 1  (e >= 1 here) */
            f = ((seq_t)3 * (em - (seq_t)1) * em) + (seq_t)1;
        }
        else
        {
            /* a(p^e) = (e+1)*(e+2)/2  for odd primes (e >= 1) */
            f = ((em + (seq_t)1) * (em + (seq_t)2)) / (seq_t)2;
        }
        new_cache[i] = new_cache[m] * f;
    }

    /* Replace old tables with the new ones.  */
    free(seq_cache);
    free(spf_tbl);
    seq_cache = new_cache;
    spf_tbl   = new_spf;
    cache_cap = new_cap + 1;   /* indices 0..new_cap inclusive */
    spf_cap   = new_cap + 1;
}

/* External linkage required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, return 0 for safety */
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
