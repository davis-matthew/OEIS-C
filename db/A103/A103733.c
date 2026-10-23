/*
 * A103733  Column k=6 sequence of array A103728.
 * https://oeis.org/A103733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (p(n+4)-1)*binomial(p(n+4)-1,6)) / p(n+4)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the k-th prime (1‑based) and binomial is the usual binomial coefficient */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Prime number generator (lazy, incremental).
   The array 'primes' stores the list of primes found so far.
   ------------------------------------------------------------------ */
static uint64_t *primes = NULL;   /* dynamic array of prime numbers */
static idx_t primes_cnt = 0;      /* number of primes currently stored */
static size_t primes_cap = 0;     /* capacity of the array */

/* add a new prime to the array, expanding storage if needed */
static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);   /* memory allocation failed */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* ensure that the prime array contains primes up to index 'idx' (0‑based) */
static void ensure_prime_index(idx_t idx)
{
    /* generate primes until we have at least idx+1 primes */
    while (primes_cnt <= idx) {
        uint64_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        while (true) {
            bool isPrime = true;
            for (idx_t j = 0; j < primes_cnt; ++j) {
                uint64_t p = primes[j];
                if (p * p > cand) break;
                if (cand % p == 0) { isPrime = false; break; }
            }
            if (isPrime) break;
            ++cand;
        }
        add_prime(cand);
    }
}

/* ------------------------------------------------------------------
   Main sequence generator:
   a(n) = (1 + (p(n+4)-1)*C(p(n+4)-1,6)) / p(n+4)
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* compute the required prime: p(n+4) with 1‑based indexing
       => zero‑based array index n+3 */
    idx_t prime_idx = n + 3;
    ensure_prime_index(prime_idx);

    seq_t p = (seq_t)primes[prime_idx];   /* 1‑based indexed prime */

    /* compute binomial(p-1, 6) using the multiplicative formula */
    seq_t binom = 1;
    for (seq_t i = 1; i <= 6; ++i) {
        seq_t num = p - 7 + i;            /* p-1-6+i = p-7+i */
        /* use 128‑bit integer to avoid overflow in intermediate step */
        binom = (seq_t)((__int128)binom * num / i);
    }

    seq_t numerator = 1 + (p - 1) * binom;
    seq_t result = numerator / p;
    return result;
}
