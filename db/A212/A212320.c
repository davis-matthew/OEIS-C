/*
 * A212320  Irregular triangle: T(n, k) = k! modulo prime(n), 1<k<prime(n),
 *          1<n.
 * https://oeis.org/A212320
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) gives the n-th term of the irregular triangle T(n,k)=k! mod
 *            prime(n) (prime(n) is the n‑th prime), flattened row‑major with
 *            first term at n=2
 * Range:     u8 n <= 6003, u16 n <= 9338, u32 n <= 9338, u64 n <= 9338,
 *            u128 n <= 9338, big any n
 * Helpers:   factorial_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorial_mod.h"

/* Static storage for primes */
static uint64_t *primes = NULL;
static size_t  primes_count = 0;
static size_t  primes_cap   = 0;

/* Ensure that primes_count >= needed (i.e., we know the first 'needed'
   primes).  Generates primes incrementally using trial division. */
static void ensure_prime_count(size_t needed)
{
    while (primes_count < needed) {
        uint64_t candidate;
        if (primes_count == 0) {
            candidate = 2;
        } else {
            candidate = primes[primes_count - 1] + 1;
            if (candidate % 2 == 0) candidate++;        /* start at odd */
        }

        /* Find next prime */
        while (true) {
            bool is_prime = true;
            for (size_t j = 0; j < primes_count; ++j) {
                uint64_t p = primes[j];
                if (p * p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* Grow array if necessary */
                if (primes_count >= primes_cap) {
                    size_t new_cap = primes_cap == 0 ? 16 : primes_cap << 1;
                    uint64_t *tmp = (uint64_t *)realloc(primes,
                                                        new_cap * sizeof(uint64_t));
                    if (!tmp) abort();           /* out of memory */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_count++] = candidate;
                break;
            }
            /* Skip even numbers */
            candidate += 2;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Undefined for n < 2 – return 0 as a safe default. */
    if (n < 2) return (seq_t)0;

    idx_t target = n - 1;          /* global position starting from 1 */
    idx_t cumulative = 0;
    idx_t row = 2;                 /* the first valid row is n=2 */

    while (true) {
        /* Make sure we know the prime for this row */
        ensure_prime_count((size_t)row);
        uint64_t p = primes[row - 1];
        idx_t len = (idx_t)(p - 2);   /* number of entries in this row */

        if (cumulative + len >= target) {
            idx_t offset0 = target - cumulative - 1; /* zero‑based offset */
            uint64_t k = (uint64_t)(offset0 + 2);     /* k = offset0 + 2 */
            return factorial_mod(k, p);
        }

        cumulative += len;
        ++row;
    }
}
