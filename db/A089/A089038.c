/*
 * A089038  Nonnegative numbers k such that 2k+5 is prime.
 * https://oeis.org/A089038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A089038 – Nonnegative numbers k such that 2k+5 is prime. */
/* Implementation: a(n) = (prime(n+2)-5)/2, where prime(i) is the i‑th prime (2,3,5,7,…). */

#include <stdlib.h>
#include <stdint.h>

/* Internal prime storage */
static uint64_t *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t prime_capacity = 0;
static idx_t last_checked = 0;

/* Append a new prime, expanding the array if needed */
static void add_prime(uint64_t p) {
    if (prime_count >= prime_capacity) {
        prime_capacity = (prime_capacity == 0) ? 32 : prime_capacity * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_capacity * sizeof(uint64_t));
        if (!tmp) abort(); /* allocation failed */
        prime_list = tmp;
    }
    prime_list[prime_count++] = p;
}

/* Generate primes until we have at least `need` primes (0‑based indexing). */
static void ensure_primes(idx_t need) {
    if (prime_count >= need) return;

    /* Initial primes 2 and 3 */
    if (!prime_list) {
        prime_capacity = 32;
        prime_list = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        if (!prime_list) abort();
        add_prime(2);
        add_prime(3);
        last_checked = 3;
    }

    for (uint64_t cand = last_checked + 1; prime_count < need; ++cand) {
        if (cand < 2) continue;
        if ((cand & 1) == 0 && cand != 2) continue; /* skip even numbers */

        int is_prime = 1;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) add_prime(cand);
        last_checked = cand;
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;       /* undefined for n<1, return 0 */

    /* Need primes up to index n+1 (since primes[0]=2, primes[1]=3) */
    ensure_primes(n + 2);

    /* p = prime at position n+2 (1‑based).  primes array is 0‑based. */
    uint64_t p = prime_list[n + 1];
    seq_t k = (seq_t)(p - 5) / 2;
    return k;
}
