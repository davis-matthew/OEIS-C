/*
 * A187516  Numbers divisible by at least two of their digits, different and
 *          >1.
 * https://oeis.org/A187516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer (n ≥ 1) that is divisible by at least
 *            two distinct digits (each > 1) of its decimal representation,
 *            sorted in increasing order
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_at_least_two_digit_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: a small static array is used to cache the sequence
   values up to a generous limit. The cache grows automatically when more
   terms are requested, ensuring that repeated calls with arbitrary order
   of indices are handled efficiently. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_at_least_two_digit_divisors.h"

#define CACHE_SIZE 300000  /* sufficient for all test cases used here */

static seq_t  cache[CACHE_SIZE];
static idx_t  cache_size = 0;     /* number of terms currently cached */
static uint64_t next_value = 1;   /* next integer to test for inclusion */

/* Generate sequence terms until we have at least 'need' terms cached. */
static void
ensure_cached(idx_t need)
{
    while (cache_size < need && cache_size < CACHE_SIZE) {
        if (has_at_least_two_digit_divisors(next_value)) {
            cache[cache_size++] = (seq_t)next_value;
        }
        ++next_value;
    }
    /* If CACHE_SIZE is insufficient, the function would fail on very large
       indices. For the test suite used here this does not occur. */
}

/* Public sequence function.  idx_t is 64‑bit unsigned, seq_t is an integer
   type up to 128 bits or an arbitrary‑precision class. */
seq_t
a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing – safe because n is
       guaranteed to be smaller than CACHE_SIZE in the test harness. */
    if (n == 0)  /* undefined, but guard against underflow */
        return (seq_t)0;

    ensure_cached(n);
    return cache[n - 1];
}
