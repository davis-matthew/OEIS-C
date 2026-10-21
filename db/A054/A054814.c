/*
 * A054814  Second term p(m) of strong prime sextets: p(m)-p(m-1) >
 *          p(m+1)-p(m) > p(m+2)-p(m+1) > p(m+3)-p(m+2) > p(m+4)-p(m+3).
 * https://oeis.org/A054814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the second prime p(m) of a sextuple of consecutive
 *            primes (p(m‑4),…,p(m+1)) such that the successive gaps are
 *            strictly decreasing: p(m)‑p(m‑1) > p(m+1)‑p(m) > p(m+2)‑p(m+1) >
 *            p(m+3)‑p(m+2) > p(m+4)‑p(m+3)
 * Range:     u8 none, u16 n <= 19, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Types supplied by the build system */
extern seq_t a(idx_t n);

static size_t    sieve_limit   = 0;       /* current upper bound of sieve      */
static bool    *is_prime      = NULL;     /* sieve bitmap (size: sieve_limit+1) */
static uint32_t *prime_list    = NULL;     /* list of primes up to sieve_limit   */
static size_t    prime_count   = 0;
static seq_t   *sextet_list   = NULL;     /* computed terms of A054814          */
static size_t    sextet_count  = 0;
static size_t    max_requested = 0;        /* largest n for which we have data     */

/* Allocate and run a normal Eratosthenes sieve up to 'limit'.
   After return, 'is_prime' points to flags, prime_list holds all primes,
   and sextet_list holds all A054814 terms up to that limit.  */
static void recompute_up_to_limit(size_t limit)
{
    /* 1. Sieve */
    bool *sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!sieve) exit(1);
    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (size_t m = p * p; m <= limit; m += p)
                sieve[m] = false;

    /* 2. Store primes */
    size_t est = limit / 10 + 50;                 /* rough prime count estimate   */
    uint32_t *plist = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!plist) exit(1);
    size_t pc = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) plist[pc++] = (uint32_t)i;

    /* 3. Identify strong sextets */
    size_t est_sextet = pc;           /* upper bound for number of sextets */
    seq_t *slist = (seq_t*)malloc(est_sextet * sizeof(seq_t));
    if (!slist) exit(1);
    size_t sc = 0;
    for (size_t i = 0; i + 5 < pc; ++i)
    {
        uint32_t a0 = plist[i];
        uint32_t a1 = plist[i + 1];
        uint32_t a2 = plist[i + 2];
        uint32_t a3 = plist[i + 3];
        uint32_t a4 = plist[i + 4];
        uint32_t a5 = plist[i + 5];
        uint32_t g1 = a1 - a0;
        uint32_t g2 = a2 - a1;
        uint32_t g3 = a3 - a2;
        uint32_t g4 = a4 - a3;
        uint32_t g5 = a5 - a4;
        if (g1 > g2 && g2 > g3 && g3 > g4 && g4 > g5)
            slist[sc++] = (seq_t)a1;             /* second prime of sextet   */
    }

    /* 4. Replace old data with new data */
    free(is_prime);
    free(prime_list);
    free(sextet_list);
    is_prime   = sieve;
    prime_list = plist;
    prime_count = pc;
    sextet_list = slist;
    sextet_count = sc;
    sieve_limit   = limit;
    max_requested = sc;
}

/* Ensure that we have computed at least 'n' terms of the sequence. */
static void ensure_terms(idx_t n)
{
    if ((size_t)n <= max_requested) return;      /* already available */

    /* Start with a modest limit and double until enough sextets are found. */
    size_t limit = (sieve_limit > 0) ? sieve_limit : 3000000u; /* 3 million first */
    while (true)
    {
        recompute_up_to_limit(limit);
        if (sextet_count >= (size_t)n) break;
        limit *= 2;                                /* enlarge the sieve */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety: sequence is 1‑based   */
    ensure_terms(n);
    return sextet_list[(size_t)n - 1];
}
