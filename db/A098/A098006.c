/*
 * A098006  (p-1)/2 - phi(p-1) as p runs through the odd primes.
 * https://oeis.org/A098006
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (p-1)/2 - φ(p-1) , p = nth odd prime (offset 2)
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper supplied by the judge environment */
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------------- */
/*  Prime cache ------------------------------------------------------------ */
/* ------------------------------------------------------------------------- */
static uint64_t *odd_primes   = NULL;   /* primes >= 3             */
static size_t   primes_cnt    = 0;      /* number of primes stored */
static size_t   primes_cap    = 0;      /* allocated capacity        */

/* Ensure that we have at least (idx+1) odd primes stored.
   idx is zero‑based: idx = 0 -> first odd prime 3. */
static void
ensure_primes(size_t idx)
{
    if (primes_cnt > idx) return; /* already enough */

    /* Allocate initial storage if needed */
    if (primes_cap == 0) {
        primes_cap = 16;
        odd_primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!odd_primes) abort();
    }

    /* Generate new primes until we reach the required index */
    for ( ; primes_cnt <= idx; ++primes_cnt ) {
        uint64_t candidate;

        if (primes_cnt == 0)
            candidate = 3;              /* first odd prime          */
        else
            candidate = odd_primes[primes_cnt-1] + 2;

        /* Skip non‑primes */
        while (!is_prime(candidate))
            candidate += 2;

        /* Grow array if necessary */
        if (primes_cnt >= primes_cap) {
            size_t new_cap = primes_cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(odd_primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            odd_primes = tmp;
            primes_cap = new_cap;
        }

        odd_primes[primes_cnt] = candidate;
    }
}

/* ------------------------------------------------------------------------- */
/*  φ(m) for 64‑bit m ------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
static seq_t phi_uint64(uint64_t m)
{
    uint64_t result = m;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return (seq_t)result;
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function ------------------------------------------------- */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Behaviour for n < 2 is undefined, but return 0 for safety */
    if (n < 2) return (seq_t)0;

    /* Convert index to zero‑based index in the odd prime list:
       n = 2  -> idx = 0 (prime 3)
       n = 3  -> idx = 1 (prime 5)  etc. */
    size_t idx = (size_t)(n - 2);

    ensure_primes(idx);

    uint64_t  p   = odd_primes[idx];
    uint64_t  pm1 = p - 1;               /* p-1  */
    seq_t     phi = phi_uint64(pm1);     /* φ(p-1) */
    seq_t     half = (seq_t)(pm1 >> 1);   /* (p-1)/2 */

    return half - phi;
}
