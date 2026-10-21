/*
 * A062350  Primes having only {1, 2, 3} as digits.
 * https://oeis.org/A062350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 69, u32 n <= 6918, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes consisting exclusively of decimal digits 1, 2, and 3 (OEIS A062350) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime64.h"

/* Pre‑computed powers of 3 up to 3^20 (enough for all 64‑bit candidates). */
static const uint64_t POW3[21] = {
    1ULL,3ULL,9ULL,27ULL,81ULL,243ULL,729ULL,2187ULL,
    6561ULL,19683ULL,59049ULL,177147ULL,531441ULL,
    1594323ULL,4782969ULL,14348907ULL,43046721ULL,
    129140163ULL,387420489ULL,1162261467ULL,3486784401ULL
};

/* Generate the next candidate having only digits 1,2,3 in increasing order. */
static uint64_t nextCandidate(void) {
    static uint64_t curr_len = 1;
    static uint64_t idx_in_len = 0;

    /* Build the candidate by interpreting idx_in_len as base‑3 number. */
    uint64_t num = 0;
    uint64_t mult = 1;
    uint64_t tmp = idx_in_len;
    for (uint64_t pos = 0; pos < curr_len; ++pos) {
        uint64_t digit = (tmp % 3) + 1;   /* 0→1, 1→2, 2→3 */
        num += digit * mult;
        mult *= 10;
        tmp /= 3;
    }

    ++idx_in_len;
    if (idx_in_len == POW3[curr_len]) {
        ++curr_len;
        idx_in_len = 0;
    }
    return num;
}

/* Sequence function required by the framework. */
seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t size = 0;
    static size_t capacity = 0;

    /* Generate primes until we have the n‑th one. */
    while ((size_t)n > size) {
        uint64_t cand = nextCandidate();
        if (isPrime64(cand)) {
            if (size == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* Out of memory */
                primes = tmp;
                capacity = new_cap;
            }
            primes[size++] = (seq_t)cand;
        }
    }
    return primes[n - 1];
}
