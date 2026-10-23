/*
 * A142074  Prime number superposition a(n) = 10*A008578(2n-1) + A008578(2n).
 * https://oeis.org/A142074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 12, if n = 1; otherwise a(n) = 10*prime(2n-2) +
 *            prime(2n-1), where prime(k) denotes the k‑th prime (prime_1 = 2)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for primes. */
static seq_t *prime_arr = NULL;
static idx_t prime_cap = 0;
static idx_t prime_cnt = 0;
static idx_t next_candidate = 2;

/* Generate primes up to the k‑th prime. */
static seq_t get_prime_at(idx_t k) {
    if (k == 0) return (seq_t)0;          /* not used */
    if (prime_cnt >= k) return prime_arr[k-1];

    /* Ensure memory for primes. */
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_arr = (seq_t *)malloc(prime_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least k. */
    while (prime_cnt < k) {
        seq_t candidate = (seq_t)next_candidate;
        bool is_prime = true;

        for (idx_t j = 0; j < prime_cnt; ++j) {
            seq_t p = prime_arr[j];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_arr = (seq_t *)realloc(prime_arr, prime_cap * sizeof(seq_t));
            }
            prime_arr[prime_cnt++] = candidate;
        }

        /* Advance to next candidate: skip even numbers after 2. */
        if (next_candidate == 2) next_candidate = 3;
        else next_candidate += 2;
    }
    return prime_arr[k-1];
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)12;
    }
    idx_t idx1 = 2*n - 2;   /* index for the (2n-2)th prime */
    idx_t idx2 = 2*n - 1;   /* index for the (2n-1)th prime */

    seq_t p1 = get_prime_at(idx1);
    seq_t p2 = get_prime_at(idx2);

    return (seq_t)10 * p1 + p2;
}
