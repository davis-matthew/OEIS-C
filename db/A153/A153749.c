/*
 * A153749  Numbers k such that there are 12 digits in k^2 and for each factor
 *          f of 12 (1,2,3,4,6) the sum of digit groupings of size f is a
 *          square.
 * https://oeis.org/A153749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, bref, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A153749: numbers k such that k^2 has 12 digits and
 * for each factor f of 12 (1,2,3,4,6) the sum of the digit groups of
 * size f is a perfect square.  The function is a(idx_t n), where
 * n ≥ 1.  Implementation uses a one‑time brute force scan of all
 * possible k and stores the results in a static array. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t *terms = NULL;        /* array of found k values          */
static size_t term_count = 0;      /* number of terms stored          */
static size_t term_cap = 0;        /* allocated capacity              */

/* Return true if s is a perfect square. */
static bool is_square(seq_t s)
{
    seq_t r = floor_sqrt(s);
    return r * r == s;
}

/* Ensure the terms array can hold at least "need" elements. */
static void ensure_capacity(size_t need)
{
    if (need <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 256;
    while (new_cap < need)
        new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    term_cap = new_cap;
}

/* Generate all terms once.  A k is valid iff its square has exactly
 * 12 digits and all required sums are perfect squares. */
static void generate_terms(void)
{
    const idx_t k_start = (idx_t)316228; /* ceil(sqrt(10^11)) */
    const idx_t k_end   = (idx_t)999999; /* floor(sqrt(10^12-1)) */

    ensure_capacity(2048);           /* initial guess, will grow if needed */

    for (idx_t k = k_start; k <= k_end; ++k) {
        uint64_t k2 = (uint64_t)k * (uint64_t)k;   /* fits in 64‑bit */

        /* Extract the 12 decimal digits of k2, most significant first. */
        uint8_t d[12];
        uint64_t temp = k2;
        for (int i = 11; i >= 0; --i) {
            d[i] = (uint8_t)(temp % 10);
            temp /= 10;
        }

        /* Sum of 1‑digit groups. */
        seq_t sum1 = 0;
        for (int i = 0; i < 12; ++i) sum1 += d[i];
        if (!is_square(sum1)) continue;

        /* Sum of 2‑digit groups. */
        seq_t sum2 = 0;
        for (int i = 0; i < 12; i += 2)
            sum2 += (seq_t)d[i] * 10 + d[i + 1];
        if (!is_square(sum2)) continue;

        /* Sum of 3‑digit groups. */
        seq_t sum3 = 0;
        for (int i = 0; i < 12; i += 3)
            sum3 += (seq_t)d[i] * 100 + d[i + 1] * 10 + d[i + 2];
        if (!is_square(sum3)) continue;

        /* Sum of 4‑digit groups. */
        seq_t sum4 = 0;
        for (int i = 0; i < 12; i += 4)
            sum4 += (seq_t)d[i] * 1000 + d[i + 1] * 100 + d[i + 2] * 10 + d[i + 3];
        if (!is_square(sum4)) continue;

        /* Sum of 6‑digit groups (two groups). */
        seq_t sum6 = 0;
        /* first group d[0]..d[5] */
        sum6 += (seq_t)d[0] * 100000 + d[1] * 10000 + d[2] * 1000
               + d[3] * 100 + d[4] * 10 + d[5];
        /* second group d[6]..d[11] */
        sum6 += (seq_t)d[6] * 100000 + d[7] * 10000 + d[8] * 1000
               + d[9] * 100 + d[10] * 10 + d[11];
        if (!is_square(sum6)) continue;

        /* All checks passed; store k. */
        if (term_count == term_cap)
            ensure_capacity(term_count + 1);
        terms[term_count++] = (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior below offset doesn't matter */
        return 0;
    if (terms == NULL)
        generate_terms();

    idx_t idx = (idx_t)(n - 1);   /* convert to zero‑based index */
    if (idx >= (idx_t)term_count)
        return 0;                 /* undefined beyond known terms */
    return terms[idx];
}
