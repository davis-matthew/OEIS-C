/*
 * A153746  Numbers k such that there are 8 digits in k^2 and for each factor
 *          f of 8 (1,2,4) the sum of digit groupings of size f is a square.
 * https://oeis.org/A153746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153746:
   Numbers k such that k^2 has 8 digits and for every factor f of 8
   (f = 1, 2, 4) the sum of the digit groupings of size f is a square.
   The function returns the n‑th such k (1‑based). */

/* Computation: exhaustive search over k with 8‑digit squares,
   recording all qualifying k in a static array. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(x) -> seq_t */

/* test whether a 64‑bit integer is a perfect square */
static bool is_square_uint(uint64_t x)
{
    seq_t r = floor_sqrt((seq_t)x);
    return r * r == (seq_t)x;
}

/* dynamic array helpers */
static seq_t *terms = NULL;        /* stored terms */
static size_t term_cnt = 0;        /* number of stored terms */
static size_t term_cap = 0;        /* capacity of the array */

/* append a value to the static array, growing it if necessary */
static void append_term(seq_t val)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* simple error handling */
        terms = tmp;
        term_cap = new_cap;
    }
    terms[term_cnt++] = val;
}

/* generate all terms the first time the function is called */
static void generate_terms(void)
{
    /* k such that k^2 has exactly 8 decimal digits:
       10^7 ≤ k^2 ≤ 10^8 – 1  =>  ceil(10^3.5) = 3163  ≤ k ≤ 9999  */
    const uint64_t k_start = 3163;
    const uint64_t k_end = 9999;

    for (uint64_t k = k_start; k <= k_end; ++k) {
        seq_t sq = (seq_t)k * k;          /* 8‑digit square */

        /* extract the 8 digits (most‑significant first) */
        uint8_t d[8];
        seq_t tmp = sq;
        for (int i = 7; i >= 0; --i) {
            d[i] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }

        /* sum of single digits (f = 1) */
        uint32_t s1 = 0;
        for (int i = 0; i < 8; ++i) s1 += d[i];
        if (!is_square_uint(s1)) continue;

        /* sum of 2‑digit groupings (f = 2) */
        uint32_t s2 = 0;
        for (int i = 0; i < 8; i += 2) {
            uint32_t g = d[i] * 10 + d[i + 1];
            s2 += g;
        }
        if (!is_square_uint(s2)) continue;

        /* sum of 4‑digit groupings (f = 4) */
        uint32_t s4 = 0;
        uint32_t left  = d[0] * 1000 + d[1] * 100 + d[2] * 10 + d[3];
        uint32_t right = d[4] * 1000 + d[5] * 100 + d[6] * 10 + d[7];
        s4 = left + right;
        if (!is_square_uint(s4)) continue;

        /* all conditions satisfied */
        append_term((seq_t)k);
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (!terms) generate_terms();          /* lazy generation */

    if (n == 0 || (idx_t)n > term_cnt)
        return (seq_t)0;                  /* out of range return 0 */

    return terms[n - 1];
}
