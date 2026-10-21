/*
 * A077338  a(n) = smallest multiple of prime(n) such that a(n) +1 is a
 *          multiple of prime(n+1).
 * https://oeis.org/A077338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 64, u32 n <= 6674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modinv_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077338: a(n) = smallest multiple of prime(n) such that a(n)+1 is a multiple of prime(n+1)  */
/* Sequence starts from n = 1.  For n = 0 the function returns 0. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/modinv_uint32.h"

/* ------------ prime list generation --------------------------------- */

static uint32_t *prime_list   = NULL;   /* primes[0] = 2, ...               */
static size_t    prime_count  = 0;       /* number of primes stored          */
static size_t    prime_cap    = 0;       /* allocated capacity               */

/* Ensure that at least (n_needed+1) primes (index 0..n_needed) are stored. */
static void ensure_primes(idx_t n_needed)
{
    if (prime_count > n_needed) return;        /* already have enough */

    if (prime_cap == 0) {                      /* first allocation */
        prime_cap = 10;
        prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) return;               /* allocation failure */
    }

    /* If the array is full, double its capacity */
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap * 2;
        uint32_t *new_arr = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!new_arr) return;                  /* realloc failed – keep old */
        prime_list = new_arr;
        prime_cap  = new_cap;
    }

    /* Start generating primes if we have none yet */
    if (prime_count == 0) {
        prime_list[prime_count++] = 2;          /* first prime */
    }

    /* Candidate numbers to test; skip even numbers after 2 */
    uint32_t candidate = (prime_count == 1) ? 3 : prime_list[prime_count-1] + 2;

    while (prime_count <= n_needed) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* Ensure capacity before adding */
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap * 2;
                uint32_t *new_arr = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
                if (!new_arr) return;          /* realloc failed */
                prime_list = new_arr;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        candidate += 2;                          /* next odd candidate */
    }
}

/* ------------ modular inverse (extended Euclid) -------------------- */

/* ------------ sequence term ---------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is defined for n ≥ 1 */

    /* we need primes up to index n (0‑based) and n+1 */
    ensure_primes(n);

    uint32_t p = prime_list[n-1];        /* prime(n)  (1‑based index) */
    uint32_t q = prime_list[n];          /* prime(n+1)                 */

    uint32_t inv = modinv_uint32(p, q);  /* inverse of p modulo q      */
    uint64_t k   = ((uint64_t)(q-1) * inv) % q;   /* smallest positive k  */
    return (seq_t)(k * p);               /* a(n) = k * prime(n)      */
}
