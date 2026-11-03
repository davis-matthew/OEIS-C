/*
 * A072903  Numbers m such that the discriminant of the quadratic field
 *          Q(sqrt(m)) < m.
 * https://oeis.org/A072903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9823 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate integers m starting from 4; include m if the discriminant
   of Q(sqrt(m)) is less than m.  The discriminant depends only on the
   square‑free part m0 of m:  Δ = m0 (if m0≡1 mod 4) else Δ = 4·m0.
   Thus Δ < m ⇔ (m0≡1 mod 4 and k≥2) or (m0≠1 mod 4 and k≥3), where
   m = m0·k².  This function uses memoisation so that calls with arbitrary
   n are O(n) after the largest n requested. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff m satisfies the discriminant condition.  m fits in 64‑bit
   because we only need to handle n up to about 10 000; the sequence values
   are well below 2⁶⁴. */
static bool valid_m(uint64_t m)
{
    uint64_t temp = m;
    uint64_t kmax = 1;

    /* Extract the square part: for every prime p, while p²|temp divide it
       out and multiply kmax by p. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        while ((temp % (p * p)) == 0) {
            temp /= p * p;
            kmax *= p;
        }
    }

    /* Now temp is the square‑free part m0. */
    uint64_t m0 = temp;
    if ((m0 & 3ULL) == 1)          /* m0 ≡ 1 (mod 4) or m0 = 1 */
        return kmax >= 2;
    else                            /* m0 ≠ 1 (mod 4) */
        return kmax >= 3;
}

/* Static memoisation table.  seq_cache holds the terms a(1), a(2), … */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* allocated slots in seq_cache */
static idx_t cache_len = 0;    /* number of terms stored so far */
static uint64_t next_m = 4;    /* next integer to test for validity */

seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is offset 1; return 0 for safety */
        return 0;

    /* Grow the cache until it contains the n‑th term. */
    while (cache_len < n) {
        /* Allocate or grow the storage as needed.  The cast makes it
           compatible with both C and C++ where seq_t may be a class. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
            if (!tmp) abort();            /* unrecoverable memory failure */
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the current integer. */
        if (valid_m(next_m)) {
            seq_cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return seq_cache[n - 1];
}
