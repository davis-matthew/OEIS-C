/*
 * A072205  a(n) = (p^2 - p + 2)/2 for p = prime(n); number of squares modulo
 *          p^2.
 * https://oeis.org/A072205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n^2 - p_n + 2)/2 * where p_n is the n-th prime
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static idx_t *prime_table = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static idx_t prime_next_candidate = 3;

/* Make sure the prime table exists. */
static void init_primes(void) {
    if (!prime_table) {
        prime_cap = 16;
        prime_table = (idx_t*)malloc(prime_cap * sizeof(idx_t));
        prime_table[0] = 2;        /* the first prime */
        prime_count = 1;
    }
}

/* Simple primality test using the already known primes. */
static bool is_prime_candidate(idx_t cand) {
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_table[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that prime_table contains at least the first 'n' primes. */
static void ensure_primes(idx_t n) {
    init_primes();
    while (prime_count < n) {
        /* Find the next prime */
        while (!is_prime_candidate(prime_next_candidate))
            prime_next_candidate += 2;   /* skip even numbers */

        /* Add the found prime */
        if (prime_count == prime_cap) {
            prime_cap *= 2;
            prime_table = (idx_t*)realloc(prime_table, prime_cap * sizeof(idx_t));
        }
        prime_table[prime_count++] = prime_next_candidate;

        prime_next_candidate += 2;   /* advance to next candidate */
    }
}

/* OEIS sequence A072205 */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* off‑by‑one safety, not used */
    ensure_primes(n);
    idx_t p = prime_table[n - 1];
    seq_t ps = (seq_t)p;
    seq_t res = (ps * ps - ps + 2) / 2;
    return res;
}
