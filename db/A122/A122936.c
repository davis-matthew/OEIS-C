/*
 * A122936  2-Round numbers: numbers n such that every number less than n and
 *          relatively prime to n has at most two prime factors (counting
 *          multiplicities).
 * https://oeis.org/A122936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate 2‑round numbers by testing each integer for the 2‑prime‑factor property.
   The sequence is finite; all terms are generated once and cached. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool is_two_round(idx_t m) {
    for (idx_t p = 2; ; ++p) {
        if (!is_prime(p))
            continue;
        if (m % p != 0) {
            seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
            return p3 >= (seq_t)m;
        }
    }
}

/* cache for the 2‑round numbers */
static seq_t cache[100];
static size_t cache_len = 0;
static const idx_t MAX_LIMIT = 5000;   /* enforces all existing terms */

seq_t a(idx_t n) {
    if (cache_len == 0) {
        for (idx_t m = 1; m <= MAX_LIMIT; ++m) {
            if (is_two_round(m))
                cache[cache_len++] = (seq_t)m;
        }
    }
    if (n == 0 || n > cache_len)
        return 0;
    return cache[n - 1];
}
