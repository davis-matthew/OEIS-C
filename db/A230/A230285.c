/*
 * A230285  a(n) = n*prime(prime(n)) - prime(n).
 * https://oeis.org/A230285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 5920, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*prime(prime(n)) - prime(n)
 *             (prime(k) is the k-th prime, with prime(1)=2)
 *
 * This implementation keeps a static list of primes and expands it
 * whenever a larger index is required.  The prime upper bound used
 * is the classical Rosser‑Schoenfeld estimate.
 *
 * The code is valid C11 and C++17; pointer casts are explicit to
 * avoid conversion warnings in C++.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/_prime_upper_bound.h"

/* static storage for primes */
static uint64_t *primes      = NULL;   /* array of primes */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static size_t   primes_cap   = 0;      /* allocated capacity */

/* sieve of Eratosthenes up to 'limit', filling the static prime array */
static void _build_primes_up_to(size_t limit)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) exit(1);

    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* enlarge storage if needed */
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 1024;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ensure at least 'needed' primes have been generated */
static void _ensure_primes(size_t needed)
{
    if (primes_cnt >= needed) return;
    size_t limit = _prime_upper_bound(needed);
    _build_primes_up_to(limit);
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, harmless */

    /* 1st prime: prime(n) */
    _ensure_primes((size_t)n);
    uint64_t pn = primes[n - 1];

    /* 2nd prime: prime(prime(n)) */
    _ensure_primes((size_t)pn);
    uint64_t ppn = primes[pn - 1];

    /* a(n) = n * ppn - pn */
    seq_t res = ( (seq_t)n * (seq_t)ppn ) - (seq_t)pn;
    return res;
}
