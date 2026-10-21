/*
 * A093667  a(n) = prime(n) - prime(n+1) + prime(n+2).
 * https://oeis.org/A093667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - prime(n+1) + prime(n+2)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A093667)                                 */
/* --------------------------------------------------------------------- */
/*  This implementation keeps a small dynamic cache of primes and grows
    it lazily up to the required index.  For primality testing the helper
    function `is_prime` from utils/is_prime.h is used.  All arithmetic is
    performed in the type seq_t so the function works for arbitrary
    precision builds. */
/* --------------------------------------------------------------------- */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

static uint64_t *prime_cache    = NULL;   /* 1‑based prime cache */
static size_t    prime_cnt      = 0;      /* number of primes cached   */
static size_t    prime_cap      = 0;      /* allocated capacity       */

/* Ensure that prime_cache contains at least `need` primes. */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;                           /* nothing to do */

    /* initialise cache if it is empty */
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cache[0] = (uint64_t)2;     /* first prime */
        prime_cnt = 1;
    }

    /* starting point for searching the next prime */
    uint64_t candidate = (prime_cnt > 0) ? prime_cache[prime_cnt-1] + 1 : 2;
    if (candidate <= 2)
        candidate = 2;
    else if (candidate % 2 == 0)
        candidate++;                      /* ensure odd after 2 */

    while (prime_cnt < (size_t)need) {
        if (is_prime(candidate)) {         /* fast primality test */
            if (prime_cnt == prime_cap) {  /* grow the cache */
                prime_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache,
                                prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_cnt++] = candidate;
        }
        candidate += 2;                    /* next odd candidate */
    }
}

/* Return the prime with 1‑based index `idx`. */
static seq_t
get_prime(idx_t idx)
{
    if (idx == 0)
        return 0;                         /* defensive, should not happen */
    ensure_primes(idx);
    return (seq_t)prime_cache[idx-1];
}

/* --------------------------------------------------------------------- */
/*  Public function required by the test harness.                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0)          /* undefined for non‑positive indices */
        return 0;

    seq_t p1 = get_prime(n);     /* prime(n)   */
    seq_t p2 = get_prime(n+1);   /* prime(n+1) */
    seq_t p3 = get_prime(n+2);   /* prime(n+2) */

    return p1 - p2 + p3;          /* term value   */
}
