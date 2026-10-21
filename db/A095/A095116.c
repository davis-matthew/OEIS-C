/*
 * A095116  a(n) = prime(n) + n - 1.
 * https://oeis.org/A095116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + n - 1
 * Range:     u8 n <= 46, u16 n <= 6018, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime storage and state */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Generate primes until at least N primes have been stored. */
static void ensure_primes_at_least(idx_t N)
{
    if (prime_count >= N) return;

    idx_t cand = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

    while (prime_count < N) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            idx_t p = prime_list[j];
            if (p > cand / p) break;            /* p^2 > cand → stop */
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 4;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_count++] = cand;
        }
        ++cand;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1, but safe */
    ensure_primes_at_least(n);
    return (seq_t)prime_list[n-1] + ((seq_t)n - 1);
}
