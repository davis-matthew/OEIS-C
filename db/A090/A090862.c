/*
 * A090862  Smallest b > 10 such that the decimal representation of the n-th
 *          prime is not a prime in base b representation.
 * https://oeis.org/A090862
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 6642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, digits_of,
 *            isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A090862:
   a(n) = smallest integer b > 10 such that the decimal
   representation of the n-th prime, interpreted as a number
   in base b, is not prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes precomputed */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt  = 0;

    if (prime_cnt < (size_t)n) {
        idx_t limit = _prime_limit_for_idx(n);
        uint64_t *new_list = NULL;
        size_t new_cnt = 0;

        if (prime_list) free(prime_list);
        build_primes_upto(limit, &new_list, &new_cnt);
        prime_list = new_list;
        prime_cnt  = new_cnt;
    }

    uint64_t p = prime_list[n - 1];

    /* Extract decimal digits of p in most‑significant‑first order */
    int len   = digits_of(p);
    uint8_t digits[20];        /* 20 digits are enough for 64‑bit numbers */
    uint64_t tmp = p;
    for (int i = len - 1; i >= 0; --i) {
        digits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Search for the minimal base b > 10 that yields a composite value */
    for (seq_t base = 11; ; ++base) {
        seq_t val = 0;
        for (int i = 0; i < len; ++i) {
            val = val * base + (seq_t)digits[i];
        }
        if (!isPrimeGeneric(val)) {
            return base;
        }
    }
}
