/*
 * A105455  Numbers k such that k*prime(k)+(k+1)*prime(k+1)+(k+2)*prime(k+2)
 *          is prime.
 * https://oeis.org/A105455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the k such that
 *            k·prime(k)+(k+1)·prime(k+1)+(k+2)·prime(k+2) is prime. Indexed
 *            from 1. The function returns the n‑th such k. It works for any
 *            call order and is efficient for n up to ~10 000
 * Range:     u8 n <= 19, u16 n <= 2273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/isPrime64.h"

/* ---------- Prime management ---------- */
static uint64_t *primes_arr = NULL;   /* 1‑based: primes_arr[0] = 2   */
static size_t   primes_cap  = 0;
static size_t   primes_size = 0;
static uint64_t next_prime_candidate = 3;

/* Make sure we have at least 'needed' primes in primes_arr. */
static void ensure_prime_count(size_t needed)
{
    if (primes_size == 0) {
        primes_cap = 16;
        primes_arr = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes_arr) exit(EXIT_FAILURE);
        primes_arr[0] = 2;
        primes_size = 1;
    }

    while (primes_size < needed) {
        while (!isPrime64(next_prime_candidate))
            next_prime_candidate += 2;

        if (primes_size >= primes_cap) {
            size_t new_cap = primes_cap * 2;
            uint64_t *tmp = (uint64_t*)realloc(primes_arr, new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes_arr = tmp;
            primes_cap = new_cap;
        }
        primes_arr[primes_size++] = next_prime_candidate;
        next_prime_candidate += 2;
    }
}

/* ---------- Sequence storage ---------- */
static uint64_t *seq_vals = NULL;   /* stores the k values */
static size_t   seq_cap  = 0;
static size_t   seq_size = 0;
static idx_t    next_k_to_test = 1;  /* next k candidate to check */

/* Expand the sequence until it contains at least 'target' terms. */
static void extend_sequence(idx_t target)
{
    while (seq_size < (size_t)target) {
        idx_t k = next_k_to_test++;               /* candidate k (1‑based) */

        /* Ensure we can access prime(k), prime(k+1), prime(k+2). */
        ensure_prime_count((size_t)k + 2);

        uint64_t p0 = primes_arr[(size_t)k - 1];
        uint64_t p1 = primes_arr[(size_t)k];
        uint64_t p2 = primes_arr[(size_t)k + 1];

        /* Compute the linear combination in 128‑bit to avoid overflow. */
        unsigned __int128 s128 =
            (unsigned __int128)k      * p0 +
            (unsigned __int128)(k+1)  * p1 +
            (unsigned __int128)(k+2)  * p2;

        /* The sum is comfortably < 2^64 for the intended range. */
        uint64_t sum64 = (uint64_t)s128;

        if (isPrime64(sum64)) {
            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                uint64_t *tmp = (uint64_t*)realloc(seq_vals, new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq_vals = tmp;
                seq_cap = new_cap;
            }
            seq_vals[seq_size++] = (uint64_t)k;
        }
    }
}

/* ---------- Public entry function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset 1 – undefined for n < 1 */
    extend_sequence(n);            /* Ensure we have at least n terms */
    return (seq_t)seq_vals[n - 1];
}
