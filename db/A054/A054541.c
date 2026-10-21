/*
 * A054541  Sum of first n terms equals n-th prime.
 * https://oeis.org/A054541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = prime(n) - prime(n-1) for n > 1
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamic array of 64‑bit primes (enough for all n ≤ 10000).
   The array is filled lazily as a() is called. */
static uint64_t *prime_arr = NULL;      /* array of primes */
static idx_t    prime_len = 0;          /* number of primes stored */
static idx_t    prime_cap = 0;          /* current capacity */

/* Grow the prime array to hold at least 'new_cap' elements. */
static void grow_prime_arr(idx_t new_cap) {
    if (new_cap <= prime_cap) return;
    prime_cap = new_cap > prime_cap ? new_cap : prime_cap * 2;
    if (prime_cap < 2) prime_cap = 2;
    prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
}

/* Append a new prime to the array. */
static void add_prime(uint64_t p) {
    grow_prime_arr(prime_len + 1);
    prime_arr[prime_len++] = p;
}

/* Ensure that at least 'needed' primes are stored in prime_arr. */
static void ensure_primes(idx_t needed) {
    if (prime_len >= needed) return;            /* already have enough */

    if (prime_len == 0) {                       /* first prime 2 */
        add_prime(2);
    }

    /* Find primes until we have 'needed' of them. */
    uint64_t candidate = prime_arr[prime_len - 1] + 1;
    if (candidate % 2 == 0) ++candidate;         /* make it odd */

    while (prime_len < needed) {
        /* Test divisibility only by known primes up to sqrt(candidate). */
        int is_prime = 1;
        for (idx_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            add_prime(candidate);
        }
        candidate += 2;  /* skip even numbers */
    }
}

/* Sequence A054541 */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* safety: undefined for n=0 */
    if (n == 1) return (seq_t)2; /* first term given explicitly */

    /* Make sure we have the first n primes. */
    ensure_primes(n);

    /* Return the difference between consecutive primes. */
    uint64_t diff = prime_arr[n - 1] - prime_arr[n - 2];
    return (seq_t)diff;
}
