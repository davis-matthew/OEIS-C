/*
 * A062340  Primes whose sum of digits is a multiple of 5.
 * https://oeis.org/A062340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th prime whose sum of decimal digits is a multiple of 5
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by incremental prime generation and digit‑sum test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, undefined for 0 */

    /* static state for memoisation */
    static seq_t   *seq  = NULL;     /* array of qualifying primes */
    static size_t    seq_len  = 0;   /* number of elements stored */
    static size_t    seq_cap  = 0;   /* allocated capacity */

    static uint64_t *primes = NULL;  /* list of all primes found so far */
    static size_t    primes_len  = 0;
    static size_t    primes_cap  = 0;

    static uint64_t last_candidate = 1; /* last integer checked */

    /* initialise storage on first call */
    if (seq == NULL) {
        seq_cap = 1024;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;               /* out of memory */

        primes_cap = 2;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) { free(seq); return 0; }
    }

    /* generate primes until we have at least n qualifying entries */
    while (seq_len < n) {
        uint64_t candidate = ++last_candidate;
        bool is_prime = true;

        /* trial division by known primes */
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > candidate / p) break;   /* p*p > candidate */
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* ensure capacity for primes list */
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(seq); free(primes); return 0; }
                primes = tmp;
            }
            primes[primes_len++] = candidate;

            /* compute digit sum modulo 5 */
            uint64_t sum = 0;
            uint64_t x = candidate;
            while (x > 0) {
                sum += x % 10;
                x /= 10;
            }
            if (sum % 5 == 0) {
                /* ensure capacity for result array */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) { free(seq); free(primes); return 0; }
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)candidate;
            }
        }
    }

    return seq[n - 1];   /* zero‑based indexing */
}
