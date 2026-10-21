/*
 * A069806  Smallest k > n such that the squarefree part of n divides the
 *          squarefree part of k.
 * https://oeis.org/A069806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that squarefree_part(n) divides
 *            squarefree_part(k)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple lazy prime sieve; works for n up to ~10 000 in well
   under two seconds per call. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t limit,
                                           uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t n); */

/* Static cache of primes */
static uint64_t *primes   = NULL;
static size_t   primes_n = 0;
static uint64_t primes_limit = 0;

/* Ensure that 'primes' contains all primes ≤ limit */
static void ensure_primes(uint64_t limit) {
    if (limit <= primes_limit)
        return;
    uint64_t *new_primes = NULL;
    size_t new_n = 0;
    build_primes_upto(limit, &new_primes, &new_n);
    free(primes);
    primes = new_primes;
    primes_n = new_n;
    primes_limit = limit;
}

/* Compute the square‑free part of a 64‑bit integer */
static uint64_t squarefree_part_u64(uint64_t n) {
    if (n == 0)
        return 0;
    uint64_t m = n;
    uint64_t sqf = 1;
    /* Need primes up to sqrt(m) */
    uint64_t lim = _floor_isqrt_u64(m) + 1;
    ensure_primes(lim);
    for (size_t i = 0; i < primes_n; ++i) {
        uint64_t p = primes[i];
        if (p > lim || p * p > m)
            break;
        if (m % p == 0) {
            int parity = 0;
            while (m % p == 0) {
                m /= p;
                parity ^= 1;
            }
            if (parity)
                sqf *= p;
        }
    }
    if (m > 1)          /* remaining prime factor (exponent 1) */
        sqf *= m;
    return sqf;
}

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t un = (uint64_t)n;
    uint64_t sqf_n = squarefree_part_u64(un);

    /* if n itself is a perfect square, its square‑free part is 1,
       so the next integer always satisfies the condition */
    if (sqf_n == 1)
        return (seq_t)(un + 1);

    uint64_t k = un + 1;
    for (;;) {
        uint64_t sqf_k = squarefree_part_u64(k);
        if (sqf_k % sqf_n == 0)
            return (seq_t)k;
        ++k;
    }
}
