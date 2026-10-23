/*
 * A040153  Primes p such that x^4 = 22 has no solution mod p.
 * https://oeis.org/A040153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that the congruence x⁴ ≡ 22 (mod p)
 *            has no solution. For a prime p ≡ 3 (mod 4) this is equivalent to
 *            (22|p) = –1, and for p ≡ 1 (mod 4) it is equivalent to
 *            22^{(p‑1)/4} ≠ 1 (mod p). The function computes the terms by
 *            incremental prime testing and stores the found prim...
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/mod_pow.h"

/* Return true iff the odd prime p has no quartic residue 22 modulo p. */
static inline bool good_prime(seq_t p)
{
    uint64_t up = (uint64_t)p;          /* p fits in 64 bits */
    if (up % 4 == 3) {
        return jacobi_symbol(22, up) == -1;
    } else { /* up % 4 == 1 */
        uint64_t exp = (up - 1) / 4;
        seq_t r = mod_pow((seq_t)22, exp, p);
        return r != (seq_t)1;
    }
}

/* Static cache of found primes.  The cache and its size are static so
   subsequent calls can reuse already computed values. */
static seq_t *cache = NULL;
static size_t capacity = 0;
static size_t count = 0;
static uint64_t candidate = 3;   /* next odd number to test */

/* seq_t a(idx_t n): return the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (count < n) {
        uint64_t p = candidate;
        if (is_prime(p) && good_prime((seq_t)p)) {
            if (count == capacity) {
                capacity = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[count++] = (seq_t)p;
        }
        candidate += 2;   /* skip even numbers */
    }
    return cache[n - 1];
}
