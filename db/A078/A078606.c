/*
 * A078606  Constant c(p) used in determining divisibility by the n-th prime,
 *          p=A000040(n), for n>=4.
 * https://oeis.org/A078606
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = c(p) where p = A000040(n) is the nth prime (with offset
 *            4). c(p) is the inverse of 10 modulo p that has the smallest
 *            absolute value
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   inverse_of_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/inverse_of_10.h"

/* Dynamic array of primes, 1‑based indexing: prime_list[i-1] = ith prime. */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Make sure prime_list has at least cap entries. */
static void grow_prime_array(void) {
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap << 1;
    prime_list = (idx_t *)realloc(prime_list, sizeof(idx_t) * new_cap);
    prime_cap = new_cap;
}

/* Test whether cand is prime using the primes already in prime_list. */
static int isprime_candidate(idx_t cand) {
    idx_t limit = cand >> 1;                     /* over‑approximation of sqrt(cand) */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* Ensure that prime_list contains at least n primes. */
static void ensure_primes_upto(idx_t n) {
    if (prime_count >= n) return;

    /* Initialise the list if empty. */
    if (prime_count == 0) {
        grow_prime_array();
        prime_list[prime_count++] = 2;
        prime_list[prime_count++] = 3;
    }

    idx_t cand = prime_list[prime_count - 1] + 1;
    if ((cand & 1) == 0) cand++;                 /* start with an odd candidate */

    while (prime_count < n) {
        if (prime_count == prime_cap) grow_prime_array();

        if (isprime_candidate(cand))
            prime_list[prime_count++] = cand;
        cand += 2;
    }
}

/* Sequence function. */
seq_t a(idx_t n) {
    if (n < 4) return 0;                     /* undefined below the offset */

    ensure_primes_upto(n);
    idx_t p = prime_list[n - 1];              /* nth prime (1‑based) */

    int64_t inv = inverse_of_10(p);           /* 0 <= inv < p */
    int64_t half = (int64_t)p >> 1;           /* floor(p/2) */

    /* Choose the representative with smallest absolute value. */
    if (inv > half) inv -= (int64_t)p;

    return (seq_t)inv;
}
