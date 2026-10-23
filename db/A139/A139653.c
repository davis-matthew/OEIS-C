/*
 * A139653  Primes of the form x^2 + 253*y^2.
 * https://oeis.org/A139653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as x² + 253·y² (offset
 *            1)
 * Range:     u8 none, u16 n <= 18, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/sqrt_floor.h"

/* Allowed residues modulo 1012 (from the OEIS note). */
static const idx_t residues[] = {
    1,  9, 25, 49, 81, 93, 133, 141, 169, 177,
    185, 213, 225, 257, 265, 269, 289, 301, 317,
    353, 357, 361, 377, 397, 441, 445, 449, 485,
    489, 509, 533, 537, 553, 565, 577, 581, 625,
    653, 669, 685, 729, 749, 785, 817, 829, 837,
    841, 905, 929, 933, 949, 961, 969, 993, 1005
};
#define RESIDUE_COUNT (sizeof(residues)/sizeof(residues[0]))

/* Cache for already found terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  last_checked = 1;   /* last tested candidate (inclusive) */

/* Check whether a prime p can be written as x² + 253·y². */
static bool has_representation(seq_t p)
{
    const seq_t d = (seq_t)253;
    if (p < (seq_t)5) return false;

    seq_t max_y = sqrt_floor(p / d);

    for (seq_t y = 0; y <= max_y; ++y) {
        seq_t yy = y * y;
        seq_t r  = p - d * yy;
        seq_t sq = sqrt_floor(r);
        if (sq * sq == r) return true;
    }
    return false;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined offset 0 */

    /* Extend cache until it contains at least n entries. */
    while (cache_len < n) {
        idx_t cand = last_checked;      /* start searching after last_checked */
        for (;;) {
            ++cand;

            /* Skip even numbers > 2. */
            if (cand > 2 && (cand & 1U) == 0) continue;

            seq_t p = (seq_t)cand;

            /* Quick congruence test: p mod 1012 must be in residues. */
            idx_t rem = (idx_t)(p % (idx_t)1012);
            bool ok = false;
            for (size_t i = 0; i < RESIDUE_COUNT; ++i) {
                if (rem == residues[i]) { ok = true; break; }
            }
            if (!ok) continue;

            /* Test primality. */
            if (!is_prime_big(p)) continue;

            /* Test representation. */
            if (has_representation(p)) {
                /* Allocate more space if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
                last_checked = cand;
                break;  /* proceed to next required term */
            }
        }
    }

    return cache[n - 1];
}
