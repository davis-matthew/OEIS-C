/*
 * A087762  Primes whose reversal is a multiple of 7.
 * https://oeis.org/A087762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 948, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as the list of primes whose decimal reversal
 * is a multiple of 7.  We compute all primes up to a fixed limit
 * (2 000 000).  There are 148 933 primes below this bound, which
 * gives more than enough terms (≈ 21 160) for all requested indices
 * up to 10 000.  After generating the primes we collect the desired
 * ones in an array and simply look them up.  This is an O(N) algorithm
 * with a small constant factor. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_num.h"

seq_t a(idx_t n) {
    /* static state, initialized on first call */
    static const idx_t LIMIT = 2000000;        /* upper bound for primes */
    static bool initialized = false;
    static uint8_t *sieve = NULL;
    static size_t sieve_size = 0;
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    static seq_t *results = NULL;
    static size_t results_cnt = 0;
    static size_t results_cap = 0;

    if (!initialized) {
        /* allocate sieve */
        sieve_size = LIMIT + 1;
        sieve = (uint8_t *)malloc(sieve_size);
        if (!sieve) return 0;
        for (size_t i = 0; i < sieve_size; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (size_t p = 2; p * p <= LIMIT; ++p) {
            if (sieve[p]) {
                for (size_t mult = p * p; mult <= LIMIT; mult += p) {
                    sieve[mult] = 0;
                }
            }
        }
        /* collect primes into dynamic array */
        primes_cap = 1 << 10;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        primes_cnt = 0;
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (sieve[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                }
                primes[primes_cnt++] = (seq_t)i;
            }
        }
        /* collect qualifying primes into results array */
        results_cap = 1 << 10;
        results = (seq_t *)malloc(results_cap * sizeof(seq_t));
        results_cnt = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            seq_t r = reverse_num(p);
            if (r % 7 == 0) {
                if (results_cnt == results_cap) {
                    results_cap <<= 1;
                    results = (seq_t *)realloc(results, results_cap * sizeof(seq_t));
                }
                results[results_cnt++] = p;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > results_cnt) return 0;
    return results[n - 1];
}
