/*
 * A088879  Numbers n such that 3n + 5 is a prime.
 * https://oeis.org/A088879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer m such that 3m + 5 is prime (OEIS A088879). */
/* Sequence is indexed from 1: a(1) = -1, a(2) = 0, a(3) = 2, a(4) = 4, ... */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Return the n-th number m with 3m+5 prime. */
seq_t a(idx_t n)
{
    /* Handle the first two special terms explicitly */
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)0;

    /* Count of terms found so far (we already have 2) */
    idx_t count = 2;
    /* Candidate integer to test, starting from 2 (first positive even to check) */
    idx_t k = 2;

    for (;;) {
        /* 3k + 5 fits comfortably in uint64_t for the n ranges considered. */
        uint64_t cand = (uint64_t)k * 3ULL + 5ULL;
        if (is_prime_small(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
