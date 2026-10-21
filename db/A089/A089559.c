/*
 * A089559  Nonnegative numbers n such that 2*n + 15 is prime.
 * https://oeis.org/A089559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089559: Nonnegative numbers n such that 2*n + 15 is prime.
   Formula: a(n) = (p_{n+6} – 15) / 2, where p_k is the k‑th prime.
   n = 1 gives p_7 = 17 → a(1) = (17–15)/2 = 1.
*/

#include <stdlib.h>
#include <stdint.h>

/* static storage for primes */
static uint64_t *prime_tbl = NULL;   /* array of primes */
static size_t  prime_cnt  = 0;       /* number of primes currently stored */
static size_t  prime_cap  = 0;       /* allocated capacity */

/* Append one prime to the table, growing the array if necessary. */
static void append_prime(uint64_t p) {
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap * 2 + 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) { exit(1); }
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
    prime_tbl[prime_cnt++] = p;
}

/* First set of primes needed for the algorithm: 2,3,5,7,11,13. */
static void seed_primes(void) {
    if (prime_cnt == 0) {
        prime_cap = 32;
        prime_tbl = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_tbl) { exit(1); }
        prime_tbl[0] = 2;  prime_tbl[1] = 3;  prime_tbl[2] = 5;
        prime_tbl[3] = 7;  prime_tbl[4] = 11; prime_tbl[5] = 13;
        prime_cnt   = 6;
    }
}

/* Generate primes until we have at least 'need' of them. */
static void ensure_primes(size_t need) {
    seed_primes();
    if (prime_cnt >= need) { return; }

    uint64_t candidate = (prime_cnt > 0) ? prime_tbl[prime_cnt-1] + 1 : 2;

    /* Brute‑force prime generation by trial division. */
    while (prime_cnt < need) {
        if (candidate < 2) { candidate = 2; continue; }

        /* Even numbers > 2 are composite. */
        if (candidate > 2 && (candidate & 1) == 0) { ++candidate; continue; }

        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > candidate) { break; }
            if (candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            append_prime(candidate);
        }
        ++candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* We need the (n+6)‑th prime: p_{n+6} */
    size_t prime_needed = (size_t)n + 6;
    ensure_primes(prime_needed);

    uint64_t p = prime_tbl[prime_needed - 1];      /* zero‑based index */
    uint64_t val = (p - 15ULL) / 2ULL;
    return (seq_t)val;
}
