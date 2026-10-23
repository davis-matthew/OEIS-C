/*
 * A163398  Numbers m such that all numbers 10*m+(odd single-digit number) and
 *          100*m+(any 2-digit, digits coprime to 10) are composite.
 * https://oeis.org/A163398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th integer m with 10*m + d composite for all odd d
 *            ∈ {1,3,5,7,9}, and 100*m + xy composite for all two‑digit xy
 *            whose digits are coprime to 10 (i.e. xy ∈ {11,13,17,…,99} with
 *            each digit ∈ {1,3,7,9}). The sequence is computed by incremental
 *            search
 * Range:     u8 none, u16 n <= 3, u32 n <= 4, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Two‑digit numbers with both digits ∈ {1,3,7,9}. */
static const int two_digits[16] = {
    11,13,17,19,
    31,33,37,39,
    71,73,77,79,
    91,93,97,99
};

/* Cache of already found terms, large enough for n up to 10000. */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t last_m = 166;   /* start just before the first term (167) */
static bool initialized = false;

/* Test whether m satisfies the conditions. */
static bool is_good_m(seq_t m)
{
    /* 10*m + d for d = 1,3,5,7,9 must be composite. */
    for (int d = 1; d <= 9; d += 2) {
        seq_t val = (seq_t)10 * m + d;
        if (is_prime_big(val)) return false;
    }
    /* 100*m + xy for all two‑digit numbers xy with digits ∈ {1,3,7,9}. */
    for (int i = 0; i < 16; ++i) {
        seq_t val = (seq_t)100 * m + two_digits[i];
        if (is_prime_big(val)) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache_len = 0;
        last_m = 166;            /* first candidate is 167 = 166+1 */
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        seq_t candidate = last_m + 1;
        if (is_good_m(candidate)) {
            if (cache_len >= CACHE_CAP) {
                /* Should not happen for n <= 10000. */
                abort();
            }
            cache[cache_len++] = candidate;
        }
        last_m = candidate;
    }

    return cache[n - 1];
}
