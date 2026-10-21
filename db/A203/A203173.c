/*
 * A203173  Central polygonal numbers that are nontrivially the product of two
 *          central polygonal numbers.
 * https://oeis.org/A203173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203173
 *  Central polygonal numbers that are nontrivially the product of two central
 *  polygonal numbers.
 *
 *  The function generates the sequence terms on demand; it stores them
 *  in a static array so repeated calls are O(1) for already computed n.
 *  Each time the array is exhausted it continues scanning the next
 *  central polygonal numbers (k^2 + k + 1) and tests for a central divisor.
 *
 *  A number m = k^2 + k + 1 is accepted iff there exists an index s ≥ 1
 *  such that d = s^2 + s + 1 divides m and the cofactor e = m/d is also
 *  central (i.e. e = t^2 + t + 1 for some t). The factor 1 is excluded
 *  automatically because d ≥ 3.
 *  The algorithm is essentially a sieve that checks O(√k) potential
 *  divisors per k, which keeps the total cost well below the required
 *  2 seconds for n up to ∼10 000.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) */

static inline bool is_central(seq_t x)
{
    /* x is central iff 4x – 3 is a perfect square w² and (w‑1) is even. */
    if (x < 3) return false;
    seq_t y = 4 * x - 3;
    seq_t w = floor_sqrt(y);
    if (w * w != y) return false;
    /* (w-1) must be even to give integer t = (w‑1)/2. */
    if ((w - 1) & 1) return false;
    return true;
}

/* Append value to static cache, growing as necessary. */
static void append_value(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* Out‑of‑memory. */
        *cache = tmp;
        *cap = new_cap;
    }
    (*cache)[(*len)++] = val;
}

/*  Main sequence generator.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_k = 0;          /* last k processed (k ≥ 1) */

    if (n <= 0) return 0;             /* undefined for n=0 */

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Advance to next central polygonal number. */
        ++last_k;
        seq_t m = (seq_t)last_k * (seq_t)last_k + (seq_t)last_k + 1;

        /* Search for a central divisor d = s^2 + s + 1, s ≥ 1. */
        seq_t s = 1;
        while (true) {
            seq_t d = s * s + s + 1;
            /* d^2 ≤ m  ⇔  d ≤ √m  ⇔  we can stop when d^2 > m. */
            seq_t d2 = d * d;
            if (d2 > m) break;
            if (m % d == 0) {
                seq_t e = m / d;
                if (is_central(e)) {
                    append_value(&cache, &cache_len, &cache_cap, m);
                    break;          /* found one valid factorisation */
                }
            }
            ++s;
        }
    }

    return cache[n-1];
}
