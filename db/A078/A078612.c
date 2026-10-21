/*
 * A078612  Number of transitions necessary for a Turing machine to compute
 *          the differences between consecutive primes (primes written in
 *          unary), when using the instruction table below.
 * https://oeis.org/A078612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)-prime(n)+(2*prime(n)+3)*(prime(n)+1)
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic array of primes (prime_list[0] = prime(1)) */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

/* Ensure *prime_list has capacity at least new_cap */
static void grow_primes(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    size_t new_cap64 = new_cap > prime_cap * 2 ? new_cap : (prime_cap * 2 > 0 ? prime_cap * 2 : 16);
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap64 * sizeof(*prime_list));
    if (!tmp) abort();                 /* out of memory */
    prime_list = tmp;
    prime_cap  = new_cap64;
}

/* Test whether candidate is prime using the current prime list */
static bool trial_is_prime(uint64_t candidate)
{
    if (candidate < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Grow the prime list until it contains at least primes up to index need_index (0‑based) */
static void ensure_primes(idx_t need_index)
{
    while (prime_len <= (size_t)need_index) {
        uint64_t candidate;
        if (prime_len == 0) candidate = 2;
        else               candidate = prime_list[prime_len - 1] + 1;

        while (!trial_is_prime(candidate)) ++candidate;

        grow_primes(prime_len + 1);
        prime_list[prime_len++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined, but harmless */

    /* Need primes up to index n (0‑based) -> prime(n+1) is at prime_list[n] */
    ensure_primes(n);

    uint64_t p = prime_list[n - 1];
    uint64_t q = prime_list[n];

    uint64_t diff = q - p;

    /* Use 128‑bit arithmetic to avoid overflow for large primes */
    unsigned __int128 prod = (unsigned __int128)((uint64_t)2 * p + 3) *
                             (unsigned __int128)(p + 1);
    uint64_t prod64 = (uint64_t)prod;  /* truncates as required by C rules */

    uint64_t sum = diff + prod64;
    return (seq_t)sum;
}
