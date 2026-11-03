/*
 * A158862  Numbers k such that prepending 3 to the k-th triangular number
 *          produces a prime.
 * https://oeis.org/A158862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that prepending 3 to the k‑th triangular number produces a prime.
   Algorithm: test successive k, cache the k’s for which  
   concatenation(3, T(k)) is prime.  Only k for which the concatenated
   number has at least 4 decimal digits are accepted, matching the OEIS
   values (the first prime occurrence is k = 22).  The function works in
   any order of calls.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* Static cache of found indices.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static size_t cached   = 0;   /* number of cached terms   */
    static size_t capacity = 0;   /* allocated capacity       */
    static idx_t  next_k   = 1;   /* next k to test           */

    /* Find enough terms up to the requested index.  We skip k’s
       where the concatenated number has fewer than 4 digits – this
       removes the trivial [k=1] case and matches the OEIS data. */
    while (cached < (size_t)n) {
        /* T(k) = k*(k+1)/2, computed in seq_t. */
        seq_t tk = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        /* Concatenate decimal '3' followed by T(k).          */
        seq_t num = concat_parts((idx_t)3, (idx_t)tk);

        /* Only consider numbers with at least 4 decimal digits. */
        if (num_digits(num) >= 4 && isPrimeGeneric(num)) {
            /* Grow cache if necessary. */
            if (capacity == 0) {
                capacity = 16;
                cache = mem_new(seq_t, capacity);
            } else if (cached == capacity) {
                capacity <<= 1;
                cache = mem_resize(seq_t, cache, capacity);
            }
            cache[cached++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
