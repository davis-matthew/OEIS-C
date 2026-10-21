/*
 * A060258  Numbers k such that 1/prime(k) has period prime(k) - 1 and
 *          1/prime(k+1) has period prime(k+1) - 1.
 * https://oeis.org/A060258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the n‑th term of OEIS A060258: numbers k such that prime(k) and
 *            prime(k+1) are both full reptend primes (10 is a primitive root
 *            modulo each). (Offset = 1)
 * Range:     u8 n <= 9, u16 n <= 893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, grow_seq, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/factorize.h"
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Test if a prime p has 10 as a primitive root modulo p. */
static bool is_full_reptend(uint64_t p)
{
    if (p <= 5) return false;                  /* 2,3,5 are not full reptend */
    uint64_t n = p - 1;

    /* Arrays for factorization of n */
    /* 64 distinct prime factors are enough for n up to 2^64-1. */
    static uint64_t primes[64];
    static int exps[64];
    int cnt = factorize(n, primes, exps);

    for (int i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        if (mod_pow((seq_t)10, n / q, (seq_t)p) == (seq_t)1) {
            return false;                      /* order divides (p-1)/q */
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset does not matter */

    /* Static memoisation of computed terms. */
    static seq_t *terms = NULL;
    static size_t used = 0;
    static size_t cap  = 0;

    /* State for generating primes and checking full reptend property. */
    static uint64_t last_prime = 1;             /* prime before the first one */
    static bool prev_full = false;
    static idx_t prime_index = 0;              /* zero‑based index of current prime */

    /* Grow terms array as needed. */
    while (used < n) {
        uint64_t p = next_prime(last_prime);
        bool cur_full = is_full_reptend(p);

        if (prime_index > 0 && prev_full && cur_full) {
            /* The previous prime has index (prime_index - 1).
               Desired k = (prime_index - 1) + 1 = prime_index. */
            if (used == cap) grow_seq(&terms, &cap, used + 1);
            terms[used++] = (seq_t)prime_index;
        }

        prev_full = cur_full;
        last_prime = p;
        ++prime_index;
    }

    return terms[n - 1];
}
