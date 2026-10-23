/*
 * A105366  Rearrangement of positive integers according to parity of
 *          (prime(n)-1)/2 (see comments).
 * https://oeis.org/A105366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1 let pₙ be the n-th prime. Compute r = (pₙ-1)/2.
 *            If r is odd, a(n) is the next unused prime; if r is even, a(n)
 *            is the next unused composite. The sequence is a permutation of
 *            the positive integers generated this way
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* declares bool is_prime(uint64_t) */

static seq_t *seq   = NULL;          /* sequence values, 0‑based (a(1) at seq[0]) */
static idx_t seq_len = 0;
static idx_t seq_cap = 0;

static uint64_t *primes      = NULL;   /* list of primes, 0‑based (p₁ at primes[0]) */
static idx_t prime_count = 0;
static idx_t prime_cap   = 0;
static uint64_t prime_gen = 2;        /* next number to test for primality */

static uint64_t composite_next = 4;   /* next candidate for composite */
static idx_t   used_prime_idx  = 0;   /* index of next unused prime in `primes` */

static void ensure_seq_capacity(idx_t need) {
    if (need <= seq_cap) return;
    idx_t ncap = seq_cap ? seq_cap * 2 : 16;
    if (ncap < need) ncap = need;
    seq = (seq_t *)realloc(seq, ncap * sizeof(seq_t));
    seq_cap = ncap;
}

static void ensure_prime_capacity(idx_t need) {
    if (need <= prime_cap) return;
    idx_t ncap = prime_cap ? prime_cap * 2 : 16;
    if (ncap < need) ncap = need;
    primes = (uint64_t *)realloc(primes, ncap * sizeof(uint64_t));
    prime_cap = ncap;
}

static void generate_primes_upto(idx_t k) {
    while (prime_count < k) {
        while (!is_prime(prime_gen)) ++prime_gen;
        ensure_prime_capacity(prime_count + 1);
        primes[prime_count++] = prime_gen;
        ++prime_gen;
    }
}

static bool is_composite(uint64_t n) {
    return n > 1 && !is_prime(n);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;             /* safety; offset is 1 */
    if (n > seq_len) {
        /* Ensure we have enough primes for all indexes up to n */
        generate_primes_upto(n);

        /* Initialize sequence if empty */
        if (seq_len == 0) {
            ensure_seq_capacity(16);
            seq[0] = (seq_t)1;         /* a(1) = 1 */
            seq_len = 1;
        }

        /* Extend the sequence up to index n */
        ensure_seq_capacity(n);
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            /* i > 1 here */
            seq_t p_i = (seq_t)primes[i - 1];            /* i-th prime */
            bool parity_odd = (((p_i - 1) >> 1) & 1) != 0;

            seq_t val;
            if (parity_odd) { /* next unused prime */
                /* The prime list is long enough because we generated primes up to n */
                val = primes[used_prime_idx++];
            } else {          /* next unused composite */
                while (!is_composite(composite_next)) ++composite_next;
                val = composite_next;
                ++composite_next;
            }
            seq[seq_len++] = val;
        }
    }
    return seq[n - 1];
}
