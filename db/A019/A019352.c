/*
 * A019352  Primes with primitive root 26.
 * https://oeis.org/A019352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th prime p for which 26 is a primitive
   root modulo p.  It generates primes sequentially (using next_prime)
   and tests the primitive root condition with modular exponentiation. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"

/* Return true iff 26 is a primitive root modulo the odd prime p. */
static bool is_primitive_root(uint64_t p)
{
    /* 26 must be coprime to p.  Exclude the trivial cases 2 and 13. */
    if (p <= 2 || p == 13)
        return false;

    uint64_t m = p - 1;           /* order of the multiplicative group */
    uint64_t original = m;        /* keep the original value for exponents */
    uint64_t div = 2;

    /* Test each distinct prime factor q of m: if 26^((p‑1)/q) ≡ 1 (mod p)
       then p does not admit 26 as a primitive root. */
    while (div * div <= m) {
        if (m % div == 0) {
            if (mod_pow((seq_t)26, original / div, (seq_t)p) == (seq_t)1)
                return false;
            while (m % div == 0)
                m /= div;
        }
        /* Advance to the next divisor: 2,3,5,7,... */
        div = (div == 2) ? 3 : div + 2;
    }
    if (m > 1) {   /* remaining prime factor */
        if (mod_pow((seq_t)26, original / m, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Lazy static cache of already found terms. */
    static bool initialized = false;
    static uint64_t *seq_u = NULL;   /* primes satisfying the condition */
    static size_t seq_size = 0;      /* number of stored primes */
    static size_t seq_cap  = 0;      /* capacity of the array */
    static uint64_t last_prime = 1;  /* start searching just above 1 */

    if (!initialized) {
        seq_cap = 1024;
        seq_u   = (uint64_t*)malloc(seq_cap * sizeof(uint64_t));
        seq_size = 0;
        initialized = true;
    }

    /* Generate primes until we have enough terms. */
    while (seq_size < n) {
        uint64_t p = next_prime(last_prime);
        last_prime = p;
        if (is_primitive_root(p)) {
            if (seq_size >= seq_cap) {
                seq_cap *= 2;
                seq_u = (uint64_t*)realloc(seq_u, seq_cap * sizeof(uint64_t));
            }
            seq_u[seq_size++] = p;
        }
    }

    return (seq_t)seq_u[n - 1];
}
