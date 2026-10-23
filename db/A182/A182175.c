/*
 * A182175  Numbers with the property that every pair of adjacent digits sum
 *          to a prime number.
 * https://oeis.org/A182175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose adjacent decimal digits sum to a prime.
 *            Computed by brute‑force search from 0 upward
 * Range:     u8 n <= 73, u16 n <= 1914, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* Prime sums from 0 to 18 */
static const bool prime_sum[19] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false, /* 9 */
    false, /*10*/
    true,  /*11*/
    false, /*12*/
    true,  /*13*/
    false, /*14*/
    false, /*15*/
    false, /*16*/
    true,  /*17*/
    false  /*18*/
};

/* Test whether every adjacent pair of digits of x sums to a prime. */
static inline bool digits_adjacent_sum_prime(idx_t x) {
    if (x == 0) return true;
    uint8_t prev = 0, cur = 0;
    bool first = true;
    while (x > 0) {
        cur = (uint8_t)(x % 10);
        if (!first) {
            if (!prime_sum[prev + cur]) return false;
        }
        prev = cur;
        first = false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1, but return 0 */

#ifdef __cplusplus
    /* C++ build: use std::vector to ensure proper construction of seq_t objects. */
    static std::vector<seq_t> vals;
    static idx_t next_candidate = 0;
    static bool initialized = false;
    if (!initialized) {
        vals.reserve(1024);
        initialized = true;
    }
    while ((idx_t)vals.size() < n) {
        idx_t cand = next_candidate;
        if (digits_adjacent_sum_prime(cand))
            vals.push_back((seq_t)cand);
        ++next_candidate;
    }
    return vals[n - 1];
#else
    /* C build: use dynamic array with malloc/realloc.  seq_t is a plain integer type. */
    static seq_t *vals = NULL;
    static size_t vals_cap = 0;
    static size_t vals_len = 0;
    static idx_t next_candidate = 0;
    static bool initialized = false;
    if (!initialized) {
        vals_cap = 1024;
        vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        initialized = true;
    }
    while (vals_len < n) {
        idx_t cand = next_candidate;
        if (digits_adjacent_sum_prime(cand)) {
            if (vals_len == vals_cap) {
                vals_cap *= 2;
                vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
            }
            vals[vals_len++] = (seq_t)cand;
        }
        ++next_candidate;
    }
    return vals[n - 1];
#endif
}
