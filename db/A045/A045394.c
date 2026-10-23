/*
 * A045394  Primes congruent to {0, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A045394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1034, u32 n <= 1034, u64 n <= 1034,
 *            u128 n <= 1034, big any n
 * Helpers:   ensure_good_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is not congruent to 1 or 2 modulo 7
   by incremental trial division. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_good_cap.h"

/* Grow a prime list to hold at least 'need' elements. */
static void ensure_prime_cap(size_t need, size_t *cap, uint64_t **arr) {
    if (need > *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 8;
        while (newcap < need) newcap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* Simple trial‑division primality test using the prime list already found. */
static int is_prime_candidate(uint64_t x, uint64_t *primes, size_t prime_cnt) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if ((x & 1) == 0) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;          /* avoid overflow */
        if (x % p == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined but safe */

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0, prime_cnt = 0;

    static seq_t *good_list = NULL;
    static size_t good_cap = 0, good_cnt = 0;

    static uint64_t candidate = 3;        /* next odd number to test */

    /* initialise the prime list with the first prime */
    if (!prime_list) {
        ensure_prime_cap(4, &prime_cap, &prime_list);
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* generate primes until we have enough qualifying ones */
    while (good_cnt < n) {
        if (is_prime_candidate(candidate, prime_list, prime_cnt)) {
            /* store the prime */
            ensure_prime_cap(prime_cnt + 1, &prime_cap, &prime_list);
            prime_list[prime_cnt++] = candidate;

            /* check the modulo condition */
            uint64_t r = candidate % 7;
            if (r != 1 && r != 2) {
                ensure_good_cap(good_cnt + 1, &good_cap, &good_list);
                good_list[good_cnt++] = (seq_t)candidate;
            }
        }
        candidate += 2;    /* skip even numbers */
    }

    return good_list[n - 1];
}
