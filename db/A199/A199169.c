/*
 * A199169  Primes such that the sum of the squares of their digits equals the
 *          number of their digits.
 * https://oeis.org/A199169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 93, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Code searches primes whose sum of squares of decimal digits
    equals the number of digits of the prime.  It memoises found
    terms so any order of calls is supported.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* Pre‑computed squares of digits 0‑9. */
static const int digit_square[10] = {
    0*0, 1*1, 2*2, 3*3, 4*4, 5*5, 6*6, 7*7, 8*8, 9*9
};

/* Global cache of found terms.  The cache is expanded as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Length of numbers that have been completely examined. */
static size_t examined_len = 2;   /* first possible length is 2 */

/* Recursive generator for all L‑digit numbers that satisfy
   sum_{digits} (digit^2) = L.  On success, the number is stored
   into the global cache if it is prime.  */
static void generate_len_rec(int pos, int rem_sq, seq_t value, int len) {
    if (pos == len) {
        if (isPrimeGeneric(value)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = value;
        }
        return;
    }

    int start_digit = (pos == 0) ? 1 : 0;
    int remaining_positions = len - pos - 1;
    for (int d = start_digit; d <= 9; ++d) {
        int sq = digit_square[d];
        if (sq > rem_sq) continue;
        /* maximum square contribution from the remaining positions */
        int max_possible = remaining_positions * 81;
        if (rem_sq - sq > max_possible) continue;
        seq_t next_value = value * 10 + (seq_t)d;
        generate_len_rec(pos + 1, rem_sq - sq, next_value, len);
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but required by spec */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Generate all numbers of the current examined_len. */
        generate_len_rec(0, examined_len, (seq_t)0, (int)examined_len);
        examined_len++;                    /* move to next length */
    }

    return cache[n - 1];
}
