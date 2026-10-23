/*
 * A108112  Numbers k such that the string 3129k is prime.
 * https://oeis.org/A108112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the decimal concatenation of
 *            "3129" and k is a prime number
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_digits, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/isPrime64.h"   /* bool isPrime64(uint64_t n); */
#include "../utils/count_digits.h" /* idx_t count_digits(uint64_t x); */

static const uint64_t pow10_tbl[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

static seq_t *seq_vals = NULL;             /* stored k values */
static idx_t   seq_cap  = 0;               /* allocated capacity */
static idx_t   seq_len  = 0;               /* number of stored terms */
static uint64_t next_k  = 1;               /* next k to test */

static void ensure_capacity(idx_t needed) {
    if (seq_cap >= needed) return;
    idx_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);     /* out of memory */
    if (seq_vals) {
        memcpy(new_arr, seq_vals, seq_len * sizeof(seq_t));
        free(seq_vals);
    }
    seq_vals = new_arr;
    seq_cap  = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined below offset */
    while (seq_len < n) {
        uint64_t k = next_k;
        idx_t digits = count_digits(k);
        uint64_t pow10 = pow10_tbl[digits];
        unsigned __int128 concat = (unsigned __int128)3129 * pow10 + k;
        uint64_t val = (uint64_t)concat;   /* safe for the ranges needed */
        if (isPrime64(val)) {
            ensure_capacity(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)k;
        }
        next_k++;
    }
    return seq_vals[n - 1];
}
