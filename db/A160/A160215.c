/*
 * A160215  Primes congruent to 2^k+1 (mod 2^(k+1)), where k is any even
 *          integer >=0.
 * https://oeis.org/A160215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that v₂(p−1) = 2m for some
 *            non‑negative integer m, i.e. the exponent of 2 in p−1 is even
 * Range:     u8 n <= 19, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for computed terms of the sequence */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* simple prime list used for trial division while generating terms */
static uint64_t *primes_all = NULL;
static size_t primes_len = 0, primes_cap = 0;

/* Ensure the term cache has capacity for at least 'need' elements. */
static void seq_grow(size_t need) {
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Ensure the prime list has capacity for at least 'need' elements. */
static void primes_grow(size_t need) {
    if (need <= primes_cap) return;
    size_t newcap = primes_cap ? primes_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    primes_all = (uint64_t *)realloc(primes_all, newcap * sizeof(uint64_t));
    primes_cap = newcap;
}

/* Count the number of trailing zero bits of a 64‑bit integer. */
static inline int trailing_zeros_uint64(uint64_t x) {
    int cnt = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

/* Generate sequence terms up to index 'required' (inclusive). */
static void generate_more_terms(idx_t required) {
    if (seq_len >= required) return;          /* already sufficient */

    /* one‑time initialisation of the first term and prime list */
    if (seq_vals == NULL) {
        seq_grow(required);
        seq_vals[0] = (seq_t)2;    /* a(1) = 2 */
        seq_len = 1;

        primes_grow(1);
        primes_all[0] = 2;         /* first prime used for trial division */
        primes_len = 1;
    }

    /* static variable remembering the starting point for the next search */
    static uint64_t next_candidate = 3;
    uint64_t candidate = next_candidate;

    while (seq_len < required) {
        /* trial division test for primality */
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes_all[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* record the new prime */
            primes_grow(primes_len + 1);
            primes_all[primes_len++] = candidate;

            /* check the parity of v₂(candidate−1) */
            if ((trailing_zeros_uint64(candidate - 1) & 1) == 0) {
                seq_grow(seq_len + 1);
                seq_vals[seq_len++] = (seq_t)candidate;
            }
        }

        candidate += 2;   /* test only odd numbers */
    }

    next_candidate = candidate;   /* remember for future calls */
}

/* Public sequence element function. */
seq_t a(idx_t n) {
    generate_more_terms(n);
    return seq_vals[n - 1];
}
