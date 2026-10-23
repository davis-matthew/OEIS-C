/*
 * A029909  Starting with each nonprime number (A018252), repeatedly sum prime
 *          factors (with multiplicity) until reaching 0 or a fixed point.
 *          Then a(n) is the fixed point (or 0).
 * https://oeis.org/A029909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 437, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A029909
 *  The nth term is obtained by taking the nth non‑prime number (A018252)
 *  and repeatedly replacing it with the sum of its prime factors (with
 *  multiplicity) until a fixed point or 0 is reached.  The fixed point
 *  (or 0) is returned as the sequence value.
 *
 *  The routine uses a sieve to generate primes and non‑primes up to a
 *  fixed upper bound that is sufficient for all requested indices
 *  (n <= 10000).  All arithmetic that can overflow a 64‑bit word is
 *  performed in a 64‑bit intermediate type; the final result is cast to
 *  seq_t.  The function works with both ordinary integer types and an
 *  arbitrary‑precision seq_t provided by the build system.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Configuration                                                    */
/* --------------------------------------------------------------------- */
#define SIEVE_MAX 200000    /* upper bound for pre‑computed tables */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Static state                                                   */
/* --------------------------------------------------------------------- */
static bool         initialized = false;
static uint8_t      *is_prime   = NULL;   /* 1 = prime, 0 = composite */
static uint64_t     *prime_list = NULL;   /* list of primes up to SIEVE_MAX */
static size_t        prime_cnt  = 0;
static seq_t        *nonprimes  = NULL;   /* list of non‑prime numbers */
static size_t        nonprimes_cnt = 0;

/* --------------------------------------------------------------------- */
/*  Utility: sum of prime factors (with multiplicity) for a 64‑bit value */
/* --------------------------------------------------------------------- */
static inline seq_t
sum_prime_factors(uint64_t v)
{
    if (v <= 1) return 0;

    uint64_t sum = 0;
    /* trial division by the pre‑computed primes list */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= v; ++i) {
        uint64_t p = prime_list[i];
        while (v % p == 0) {
            sum += p;
            v   /= p;
        }
    }
    /* if a prime factor remains */
    if (v > 1) sum += v;

    return (seq_t)sum;
}

/* --------------------------------------------------------------------- */
/*  Utility: fixed point by repeated application of sum_prime_factors  */
/* --------------------------------------------------------------------- */
static inline seq_t
fixed_point(seq_t m)
{
    if ((uint64_t)m <= 1) return 0;

    for (;;) {
        seq_t next = sum_prime_factors((uint64_t)m);
        if (next == m) return m;
        m = next;
    }
}

/* --------------------------------------------------------------------- */
/*  One‑time initialization: sieve, populate prime_list, nonprimes    */
/* --------------------------------------------------------------------- */
static void
init(void)
{
    if (initialized) return;
    initialized = true;

    /* --- Sieve of Eratosthenes ------------------------------------ */
    is_prime = (uint8_t *)malloc((SIEVE_MAX + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= SIEVE_MAX; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t p = 2; p * p <= SIEVE_MAX; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= SIEVE_MAX; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* --- Count primes and non‑primes -------------------------------- */
    size_t primes_est   = 0;
    size_t nonprimes_est = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i) {
        if (is_prime[i]) ++primes_est; else ++nonprimes_est;
    }
    /* include 1 as a non‑prime */
    nonprimes_est++;

    /* --- Allocate and fill prime list ------------------------------ */
    prime_list = (uint64_t *)malloc(primes_est * sizeof(uint64_t));
    prime_cnt  = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i) {
        if (is_prime[i]) prime_list[prime_cnt++] = i;
    }

    /* --- Allocate and fill non‑prime list -------------------------- */
    nonprimes = (seq_t *)malloc(nonprimes_est * sizeof(seq_t));
    nonprimes_cnt = 0;
    /* 1 is considered non‑prime */
    nonprimes[nonprimes_cnt++] = (seq_t)1;
    for (size_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!is_prime[i]) nonprimes[nonprimes_cnt++] = (seq_t)i;
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                       /* ensure tables are ready */

    if (n == 0) return 0;         /* undefined for zero index */

    /* if the requested index exceeds pre‑computed range, extend the sieve */
    if ((idx_t)nonprimes_cnt < n) {
        /* Dynamic expansion is omitted here; for n <= 10000 the chosen
           SIEVE_MAX suffices.  If needed, one could extend the sieve
           by a larger factor and rebuild the tables. */
        return 0;
    }

    seq_t start = nonprimes[n - 1];
    return fixed_point(start);
}
