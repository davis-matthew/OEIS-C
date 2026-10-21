/*
 * A095824  Numbers k such that k + largest digit of k is prime.
 * https://oeis.org/A095824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   grow_cache, isPrime, largest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095824: numbers k such that k + largest digit of k is prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrime.h"
#include "../utils/grow_cache.h"
#include "../utils/largest_digit.h"

/* Global cache state. */
static seq_t *seq_cache   = NULL;
static idx_t seq_cache_len = 0;   /* number of cached terms */
static idx_t seq_cache_cap = 0;   /* allocated capacity */

/* The next integer to test for eligibility. */
static idx_t _next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    if (n > seq_cache_len) {
        /* Ensure sufficient capacity. */
        grow_cache(&seq_cache_cap, &seq_cache, n);

        /* Search for terms until we have at least n of them. */
        idx_t k;
        for (k = _next_candidate; seq_cache_len < n; ++k) {
            seq_t d   = largest_digit((seq_t)k);
            seq_t sum = (seq_t)k + d;
            if (isPrime((idx_t)sum)) {
                seq_cache[seq_cache_len++] = (seq_t)k;
            }
        }
        /* Store the next candidate to continue from. */
        _next_candidate = k;
    }

    return seq_cache[n - 1];
}
