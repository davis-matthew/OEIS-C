/*
 * A099794  a(n) = smallest integer k such that k*prime(n) == 1 mod j for each
 *          integer j with 1<j<prime(n).
 * https://oeis.org/A099794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the modular inverse of prime(n) modulo
 *            LCM(2,3,…,prime(n)-1). The value is obtained by solving
 *            k·prime(n) ≡ 1 (mod q^e) for each prime power q^e ≤ prime(n)-1
 *            using the Chinese Remainder Theorem
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   mod_inverse_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---  Required headers ----------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_inverse_int64.h"

/*---  Prime table – incremental generation -------------------------------*/
static uint32_t *prime_tbl = NULL;   /* array of primes, 1‑based indexing omitted */
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static void prime_grow(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    uint32_t *tmp = (uint32_t *)realloc(prime_tbl, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* Return the *n*‑th prime (1‑based).  Generates more primes as needed. */
static uint32_t nth_prime(idx_t n)
{
    if (prime_cnt >= (size_t)n) return prime_tbl[n - 1];

    /* Generate primes until we have at least n of them. */
    uint32_t candidate = (prime_cnt == 0) ? 2 : prime_tbl[prime_cnt - 1] + 1;

    while (1) {
        if (candidate > 2 && (candidate & 1) == 0) { ++candidate; continue; }

        int is_pr = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_tbl[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_pr = 0; break; }
        }

        if (is_pr) {
            prime_grow(prime_cnt + 1);
            prime_tbl[prime_cnt++] = candidate;
            if (prime_cnt >= (size_t)n) return candidate;
        }
        ++candidate;
    }
}

/*---  Main sequence function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined below offset – return 0 */

    uint32_t p = nth_prime(n);                /* prime(n) */

    if (p <= 3) return 1;                   /* a(1) = a(2) = 1 */

    uint32_t limit = p - 1;                  /* numbers to consider in LCM */

    seq_t k = 0;                              /* current CRT solution */
    seq_t M = 1;                              /* current modulus product */

    size_t idx = 0;                           /* iterate over primes */

    while (1) {
        /* Ensure we have the prime at index idx. */
        if (idx >= prime_cnt) nth_prime(idx + 1);
        uint32_t q = prime_tbl[idx];
        if (q > limit) break;                /* all required primes processed */

        /* Compute the largest power q^e ≤ limit. */
        uint64_t pow_q = q;
        while (pow_q * q <= limit) pow_q *= q;  /* pow_q fits in 64‑bit */

        uint64_t mod_q = pow_q;                 /* modulus for this congruence */

        /* Inverse of p modulo mod_q.  p and mod_q are coprime. */
        uint64_t r = (uint64_t)mod_inverse_int64(
                        (int64_t)(p % mod_q), (int64_t)mod_q);

        /* CRT combination:
           Find t such that k + M*t ≡ r (mod mod_q).
           Solve M*t ≡ (r - k) (mod mod_q). */
        uint64_t k_mod_q = (uint64_t)(k % mod_q);
        uint64_t diff    = ((uint64_t)r + mod_q - k_mod_q) % mod_q;

        uint64_t M_mod_q = (uint64_t)(M % mod_q);
        uint64_t inv_M   = (uint64_t)mod_inverse_int64(
                            (int64_t)M_mod_q, (int64_t)mod_q);

        uint64_t t = (uint64_t)(( (__int128)diff * inv_M ) % mod_q);

        /* Update solution and modulus.  Use seq_t arithmetic. */
        k = k + M * (seq_t)t;
        M = M * (seq_t)mod_q;

        ++idx;
    }
    return k;            /* minimal positive integer satisfying the condition */
}
