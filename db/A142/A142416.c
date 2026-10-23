/*
 * A142416  Primes congruent to 3 mod 49.
 * https://oeis.org/A142416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 3 modulo 49
   a(n) is the nth prime p with p % 49 == 3.
   Computed by testing successive numbers of the form 49*k + 3. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* dynamic cache of found primes */
static seq_t *prime_list = NULL;   /* array of primes (seq_t) */
static size_t prime_cnt  = 0;      /* number of primes stored */
static size_t prime_cap  = 0;      /* capacity of prime_list */
static idx_t  next_k     = 0;      /* next k for candidate 49*k + 3 */

/* enlarge prime_list to a new capacity */
static void grow_prime_list(size_t new_cap) {
    seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* nth prime congruent to 3 (mod 49) */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* offset is 1; undefined for n < 1 */

    /* generate primes until we have at least n entries */
    while (prime_cnt < n) {
        idx_t candidate = 49 * next_k + 3;   /* candidate congruent to 3 mod 49 */
        next_k++;

        /* test primality; is_prime expects idx_t */
        if (is_prime(candidate)) {
            /* grow the cache if necessary */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 32;
                grow_prime_list(new_cap);
            }
            /* store the prime as seq_t */
            prime_list[prime_cnt++] = (seq_t)candidate;
        }
    }
    /* return the (n-1)th element (0‑based index) */
    return prime_list[n - 1];
}
