/*
 * A181095  Permutation of positive integers created from sequence A008578(n)
 *          = noncomposites.
 * https://oeis.org/A181095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the permutation obtained by swapping each non‑composite
 *            (1 or a prime) with a distinct composite according to the
 *            definition given in OEIS A181095. The implementation builds a
 *            dynamic sieve of primes and uses the prime/composite indices to
 *            map each n to its partner. The formula is not a s...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------  static data  ---------------------------------- */
static uint8_t *prime_is  = NULL;          /* 1 if prime, 0 otherwise */
static idx_t  *prime_cnt = NULL;          /* prefix prime counts */
static uint64_t *prime_tbl = NULL;        /* list of primes in order */
static idx_t   prime_num = 0;             /* number of primes known      */
static idx_t   sieve_limit = 0;           /* current sieve bound          */

/* -------------  helper functions  ----------------------------- */
static void rebuild_sieve(idx_t limit)
{
    free(prime_is);
    free(prime_cnt);
    free(prime_tbl);

    prime_is  = (uint8_t*)malloc((size_t)(limit + 1));
    prime_cnt = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!prime_is || !prime_cnt)
        return;               /* out‑of‑memory – not expected */

    memset(prime_is, 1, (size_t)(limit + 1));
    if (limit >= 0) prime_is[0] = 0;
    if (limit >= 1) prime_is[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (prime_is[p])
            for (idx_t m = p * p; m <= limit; m += p)
                prime_is[m] = 0;

    idx_t c = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (i >= 2 && prime_is[i]) ++c;
        prime_cnt[i] = c;
    }

    prime_num = prime_cnt[limit];
    prime_tbl = (uint64_t*)malloc((size_t)prime_num * sizeof(uint64_t));
    idx_t q = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_is[i]) prime_tbl[q++] = (uint64_t)i;

    sieve_limit = limit;
}

static void ensure_sieve(idx_t needed)
{
    if (sieve_limit >= needed) return;

    idx_t new_limit = needed + 1000;                 /* safety padding */
    if (sieve_limit > 0 && new_limit <= sieve_limit)
        new_limit = sieve_limit * 2 + 1000;          /* exponential grow */

    rebuild_sieve(new_limit);
}

/* Return the 1‑based composite index of n (n ≥ 4) */
static idx_t composite_index(idx_t n)
{
    return n - (1 + prime_cnt[n]);                  /* composites = n - primes - 1 */
}

/* Find the composite whose 1‑based index equals target (target ≥ 1) */
static idx_t find_composite_by_index(idx_t target)
{
    idx_t lo = 4, hi = sieve_limit;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t idx_mid = composite_index(mid);
        if (idx_mid >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* -------------------------------------------------------------- */
/*  main sequence function                                         */
seq_t a(idx_t n)
{
    /* base values (a(1)=1, a(2)=2, a(3)=3) */
    if (n <= 3u)
        return (seq_t)n;

    /* choose a generous sieve limit: enough primes for composites needed
       up to n.  The factor 25 gives plenty of slack for n up to at least
       10 000; the safety padding guarantees primes for the largest
       composites considered. */
    idx_t limit_needed = n * 25u + 1000u;
    if (limit_needed < 1000u)
        limit_needed = 1000u;
    ensure_sieve(limit_needed);

    bool is_prime_n = prime_is[n] != 0;

    if (is_prime_n) {
        /* prime case */
        idx_t prime_ord = prime_cnt[n];         /* 1‑based index of this prime */
        if (prime_ord <= 2u)
            return (seq_t)n;                    /* 2 and 3 map to themselves */

        idx_t comp_idx = prime_ord - 2u;        /* j = k – 2   (1‑based)       */
        idx_t comp_val = find_composite_by_index(comp_idx);
        return (seq_t)comp_val;
    } else {
        /* composite case (n ≥ 4) */
        idx_t comp_idx = composite_index(n);    /* j   (1‑based)                */
        idx_t prime_ord = comp_idx + 2u;        /* k = j + 2 (1‑based)          */

        if (prime_ord > prime_num) {            /* need more primes */
            idx_t new_limit = sieve_limit * 2 + 1000;
            if (new_limit < limit_needed) new_limit = limit_needed;
            ensure_sieve(new_limit);
        }

        return (seq_t)prime_tbl[prime_ord - 1u]; /* 0‑based array access        */
    }
}
