/*
 * A102372  Numbers k such that k11111 is prime.
 * https://oeis.org/A102372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer k such that k*100000 + 11111 is prime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(uint64_t); */
#include "../utils/ensure_capacity.h"

/* External function to test 64‑bit primality.
   Provided by the utilities: bool is_prime(uint64_t n); */
extern bool is_prime(uint64_t);

/* Sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n < 1, return 0. */

    /* Static state spanning multiple calls. */
    static seq_t *vals      = NULL;   /* Array of found k values. */
    static size_t vals_len  = 0;       /* Current number of stored values. */
    static size_t vals_cap  = 0;       /* Current capacity of the array. */
    static idx_t next_k     = 1;       /* Next candidate k to test. */

    /* Generate more terms until we have at least n values. */
    while (vals_len < n) {
        /* Search for the next k yielding a prime N = k*100000 + 11111. */
        while (1) {
            uint64_t k = (uint64_t)next_k;               /* Safe cast; k stays < 2^64. */
            uint64_t N = k * 100000ULL + 11111ULL;       /* N fits in 64‑bit. */

            if (is_prime(N)) {
                /* Add this k to the array. */
                ensure_capacity(vals_len + 1, &vals, &vals_cap);
                vals[vals_len++] = (seq_t)next_k;
                next_k++;             /* Advance for the next search. */
                break;                 /* Term found, exit inner loop. */
            }
            next_k++;                 /* Advance candidate with no prime found. */
        }
    }

    return vals[n - 1];   /* Return the n-th term (1‑based index). */
}
