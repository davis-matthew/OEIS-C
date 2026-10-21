/*
 * A059645  Primes p such that x^60 = 2 has no solution mod p.
 * https://oeis.org/A059645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p for which the congruence x^60 ≡ 2 (mod
 *            p) has no solution. The test used is if 2^((p-1)/gcd(60,p-1))
 *            mod p ≠ 1, then p is included. The code uses the built‑in helper
 *            functions is_prime(idx_t) and mod_pow(seq_t, uint64_t, seq_t)
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Euclidean GCD for 64‑bit unsigned integers. */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure the prime cache has space for at least 'needed' entries. */
static inline void grow_primes(size_t needed, seq_t **arr, size_t *cap) {
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap) * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    /* Cache of primes satisfying the condition. */
    static seq_t *primes       = NULL;
    static size_t primes_cnt   = 0;
    static size_t primes_cap   = 0;
    static idx_t candidate     = 3;   /* first odd prime > 2 */

    while (primes_cnt < n) {
        if (is_prime(candidate)) {
            idx_t g   = gcd_u64(60, candidate - 1);
            idx_t exp = (candidate - 1) / g;
            seq_t res = mod_pow((seq_t)2, exp, (seq_t)candidate);
            if (res != (seq_t)1) {
                grow_primes(primes_cnt + 1, &primes, &primes_cap);
                primes[primes_cnt++] = (seq_t)candidate;
            }
        }
        candidate += 2;   /* next odd candidate */
    }

    return primes[n - 1];
}
