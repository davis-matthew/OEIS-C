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
 * Range:     u8 n <= 6003, u16 n <= 65221, u32 n <= 65221, u64 n <= 65221,
 *            u128 n <= 65221, big any n
 * Validated: standard, pass, 98 terms through n = 9338 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute k! mod p for small k, where k < p and p fits into uint64_t. */
static seq_t factorial_mod(uint64_t k, uint64_t p)
{
    seq_t res = (seq_t)1;
    for (uint64_t i = 2; i <= k; ++i) {
        uint64_t prod = ((uint64_t)res * i) % p;
        res = (seq_t)prod;
    }
    return res;
}

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
