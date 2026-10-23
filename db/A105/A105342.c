/*
 * A105342  Least k > 0 such that prime(n+k) == k (mod prime(n)).
 * https://oeis.org/A105342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that prime(n+k) == k (mod prime(n))
 * Range:     u8 n <= 12, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/estimate_prime_upper_bound.h"

/* static storage for sieve data */
static uint8_t  *isComposite = NULL;  /* 0 = prime, 1 = composite */
static uint32_t *primes      = NULL;  /* list of primes */
static size_t   sieve_limit  = 0;     /* current upper bound of sieve array (prime value) */
static size_t   primes_count = 0;     /* number of primes stored */

/* re‑sieve up to the value given by 'limit' (prime value) */
static void recompute_sieve(size_t limit)
{
    if (isComposite != NULL) {
        free(isComposite);
    }
    if (primes != NULL) {
        free(primes);
    }
    isComposite = (uint8_t *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) isComposite[i] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!isComposite[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                isComposite[j] = 1;
            }
        }
    }

    /* rough guess of how many primes we will have */
    size_t est = limit / 10 + 100;
    primes = (uint32_t *)malloc(sizeof(uint32_t) * est);
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!isComposite[i]) {
            if (cnt == est) {
                est = est * 3 / 2;
                primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * est);
            }
            primes[cnt++] = (uint32_t)i;
        }
    }
    primes_count = cnt;
    sieve_limit  = limit;
}

/* ensure that primes[needed_index] exists (0‑based index) */
static void ensure_primes_idx(idx_t needed_index)
{
    if (primes_count > needed_index) return;              /* already have it */

    /* estimate prime value needed for this index */
    idx_t required_n = needed_index + 1;                  /* 1‑based count of primes required */
    size_t new_limit = estimate_prime_upper_bound(required_n);

    /* safety: if estimate is too low, double the limit */
    if (new_limit <= sieve_limit) {
        new_limit = sieve_limit * 2 + 1024;
    }

    recompute_sieve(new_limit);

    /* repeat if still not enough primes (possible if estimate was too small) */
    while (primes_count <= needed_index) {
        new_limit *= 2;
        recompute_sieve(new_limit);
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;            /* defined as 0 for indices ≤0 */

    idx_t zero_idx = n - 1;                 /* convert to 0‑based index */
    ensure_primes_idx(zero_idx);

    seq_t p = (seq_t)primes[zero_idx];      /* prime(n) */

    idx_t k = 1;                            /* search starting from 1 */
    for (;;) {
        idx_t idx2 = zero_idx + k;          /* index of prime(n+k) */
        ensure_primes_idx(idx2);

        seq_t q = (seq_t)primes[idx2];      /* prime(n+k) */
        if ((q % p) == ((seq_t)k % p)) {
            return (seq_t)k;
        }
        ++k;
    }
}
