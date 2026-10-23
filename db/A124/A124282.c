/*
 * A124282  Primes indexed by 4-almost primes.
 * https://oeis.org/A124282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1091, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the prime having index equal to the n‑th number with
 * exactly four prime factors (counted with multiplicity).  The values
 * are obtained by sieving for big‑omega = 4 and for primes, then
 * indexing into the prime list.  The sieve limit is increased
 * automatically until enough terms are available. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t      current_limit = 0;   /* current sieve limit */
static uint32_t  *spf          = NULL; /* smallest prime factor array (size limit+1) */
static uint8_t   *bigomega     = NULL; /* big‑omega array (size limit+1) */
static uint32_t  *primes       = NULL; /* prime values array (size limit+1) */
static size_t     primes_cnt   = 0;    /* number of primes currently stored    */
static uint32_t  *four_arr     = NULL; /* 4‑almost‑prime numbers (size limit+1)*/
static size_t     four_cnt     = 0;    /* number of such numbers stored        */

/* ------------------------------------------------------------------ */
/* Rebuild the sieve up to the given limit.  All arrays are re‑allocated
 * and recomputed.  This is intentionally simple; the limit never
 * shrinks, only grows. */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate / re‑allocate global arrays */
    spf      = (uint32_t *)realloc(spf,      (size_t)(limit + 1) * sizeof(uint32_t));
    bigomega = (uint8_t  *)realloc(bigomega, (size_t)(limit + 1) * sizeof(uint8_t));
    primes   = (uint32_t *)realloc(primes,   (size_t)(limit + 1) * sizeof(uint32_t));
    four_arr = (uint32_t *)realloc(four_arr, (size_t)(limit + 1) * sizeof(uint32_t));

    /* Simple sieve for smallest prime factor */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = (uint32_t)i;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (spf[i] == (uint32_t)i)
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;

    /* Compute big‑omega values */
    bigomega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        bigomega[i] = bigomega[i / spf[i]] + 1;

    /* Build prime and 4‑almost‑prime lists */
    primes_cnt = 0;
    four_cnt   = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i)           /* i is prime */
            primes[primes_cnt++] = (uint32_t)i;
        if (bigomega[i] == 4)                /* i has exactly 4 prime factors */
            four_arr[four_cnt++] = (uint32_t)i;
    }

    current_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the internal data contains at least `need_terms` many
 * 4‑almost‑primes and at least the `m`‑th prime needed for the answer. */
static void ensure(idx_t need_terms)
{
    /* Expand sieve until we have enough 4‑almost‑prime values */
    if (four_cnt < need_terms || current_limit == 0) {
        idx_t limit = current_limit == 0 ? 1024 : current_limit * 2;
        while (true) {
            rebuild_sieve(limit);
            if (four_cnt >= need_terms)
                break;
            limit *= 2;
        }
    }

    /* Now we know the n‑th 4‑almost‑prime value: */
    uint32_t m = four_arr[need_terms - 1]; /* m is the index we need in the prime list */

    /* Expand sieve until the m‑th prime exists */
    if (primes_cnt < (size_t)m) {
        idx_t limit = current_limit;
        while (primes_cnt < (size_t)m) {
            limit *= 2;
            rebuild_sieve(limit);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function
 *   a(n) = prime(4almostprime(n))
 */
seq_t a(idx_t n)
{
    /* n is 1‑based; we use 0‑based indices internally */
    if (n == 0)   /* undefined for offset 0; return 0 as safe fallback */
        return (seq_t)0;

    ensure(n);

    uint32_t m = four_arr[n - 1];          /* m = 4‑almost‑prime(n) */
    return (seq_t)primes[m - 1];           /* prime index m (1‑based) */
}
