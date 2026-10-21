/*
 * A227623  Numbers k such that phi(k) + pi(k) is prime.
 * https://oeis.org/A227623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1326, u32 n <= 1326, u64 n <= 1326,
 *            u128 n <= 1326, big any n
 * Helpers:   calc_phi, compute_pi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227623 – numbers k such that φ(k)+π(k) is prime.
   Computed by a simple incremental sieve and on‑demand checks.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/calc_phi.h"
#include "../utils/compute_pi.h"
#include "../utils/is_prime.h"

/* Ensure array of element size elem_size can hold needed items. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? *cap * 2 : 16;
    while (newcap < needed)
        newcap <<= 1;
    void *newptr = realloc(*ptr, newcap * elem_size);
    if (!newptr)
        exit(EXIT_FAILURE);
    *ptr = newptr;
    *cap = newcap;
}

/* Sieve data */
static idx_t   *primes      = NULL;   /* primes found so far */
static size_t  primes_cap  = 0, primes_cnt = 0;
static idx_t   max_k = 0;             /* highest integer processed */

/* Extend the sieve up to `limit`. */
static void extend_sieve(idx_t limit)
{
    if (limit <= max_k)
        return;

    /* ensure enough space for the prime list */
    ensure_capacity(limit + 1, &primes_cap, (void **)&primes, sizeof(idx_t));

    for (idx_t i = max_k + 1; i <= limit; ++i) {
        bool prime = true;
        if (i < 2)
            prime = false;
        else {
            for (size_t p = 0; p < primes_cnt; ++p) {
                idx_t q = primes[p];
                if (q * q > i)
                    break;
                if (i % q == 0) {
                    prime = false;
                    break;
                }
            }
        }
        if (prime)
            primes[primes_cnt++] = i;
    }
    max_k = limit;
}

/* Storage for sequence values */
static seq_t *values     = NULL;   /* found terms */
static size_t values_cap = 0, values_cnt = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, guard */

    while (values_cnt < n) {
        idx_t k = max_k + 1;
        extend_sieve(k);

        /* φ(k) and π(k) */
        seq_t phi = calc_phi((seq_t)k);
        seq_t pi_k = compute_pi(k);

        /* test primality of φ(k)+π(k) */
        idx_t sum = (idx_t)phi + (idx_t)pi_k;
        if (is_prime(sum)) {
            ensure_capacity(values_cnt + 1, &values_cap, (void **)&values, sizeof(seq_t));
            values[values_cnt++] = (seq_t)k;
        }
    }
    return values[n - 1];
}
