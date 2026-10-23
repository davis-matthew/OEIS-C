/*
 * A042995  Primes congruent to {0, 2, 3, 5} (mod 7).
 * https://oeis.org/A042995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p with p mod 7 ∈ {0,2,3,5}
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static memoization storage for qualifying primes */
static idx_t *prime_list   = NULL;
static size_t prime_list_len = 0;
static size_t prime_list_cap = 0;
static idx_t next_candidate = 2;

/* ensure the prime list has enough capacity */
static void ensure_prime_capacity(void)
{
    if (prime_list_len == prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
}

/* test whether a candidate’s residue modulo 7 is allowed */
static inline bool ok_residue(idx_t x)
{
    if (x <= 7)               /* 2, 3, 5, 7 are allowed */
        return true;
    idx_t r = x % 7;
    return (r == 0) || (r == 2) || (r == 3) || (r == 5);
}

/* return the n-th prime that satisfies the modulo‑7 condition */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1 */
        return 0;

    while (prime_list_len < n) {
        idx_t cand = next_candidate;

        if (ok_residue(cand) && is_prime(cand)) {
            ensure_prime_capacity();
            prime_list[prime_list_len++] = cand;
        }

        /* Advance to the next candidate (skip evens) */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return (seq_t)prime_list[n - 1];
}
