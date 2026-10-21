/*
 * A080363  Nonprime numbers k such that the largest prime divisor of k is
 *          unitary.
 * https://oeis.org/A080363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime numbers k such that the largest prime divisor of
 *            k is unitary
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_largest_prime_unary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. k is composite and its largest prime factor appears with exponent 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_largest_prime_unary.h"

/* Maximum number of terms we cache (covers n up to about 10000) */
#define CACHE_CAP 10000

/* Cached sequence values */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;      /* number of terms already computed */
static uint64_t next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        uint64_t v = next_candidate++;
        if (is_largest_prime_unary(v)) {
            cache[cache_len++] = (seq_t)v;
        }
    }
    return cache[n - 1];
}
