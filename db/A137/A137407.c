/*
 * A137407  Numbers that cannot be the length of the long leg in any primitive
 *          Pythagorean triple.
 * https://oeis.org/A137407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primitive Pythagorean triples, mark lengths of the
 * long leg, and return the nth positive integer that is NOT marked.
 * This realizes OEIS sequence A137407.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Generate all long‑leg lengths <= limit and store them in the *mark array.
 * The array must have at least `limit+1` entries.  Marked positions receive
 * value 1, unmarked positions remain 0.  The function returns the number
 * of unmarked entries up to `limit`. */
static idx_t build_mark(idx_t limit, uint8_t *mark)
{
    /* clear previous marks */
    for (idx_t i = 1; i <= limit; ++i) mark[i] = 0;

    /* Generate primitive triples via Euclid's formula.
     * m > n >= 1, gcd(m,n)=1, m-n odd.
     * For each pair, compute the two legs a=m²-n² and b=2mn.
     * The larger of the two is the long leg.  If it does not exceed
     * `limit`, it is marked.  We stop increasing m for a fixed n
     * once both legs exceed `limit`. */
    for (idx_t n = 1; n < limit; ++n) {
        for (idx_t m = n + 1; ; ++m) {
            seq_t a = (seq_t)m * (seq_t)m - (seq_t)n * (seq_t)n;
            seq_t b = (seq_t)2 * (seq_t)m * (seq_t)n;

            if (a > limit && b > limit) break;          /* no longer useful */

            if (((m & 1) == (n & 1))) continue;        /* same parity */
            if (gcd((seq_t)m, (seq_t)n) != 1) continue; /* not coprime */

            seq_t long_leg = (a > b) ? a : b;
            if (long_leg <= limit) mark[(idx_t)long_leg] = 1;
        }
    }

    /* Count unmarked entries (the numbers that cannot be a long leg). */
    idx_t missing = 0;
    for (idx_t i = 1; i <= limit; ++i) if (!mark[i]) ++missing;
    return missing;
}

/* The core sequence function.
 * It keeps a static memoised state so that repeated calls with arbitrary
 * indices are fast.  The limit is doubled until at least `n` missing
 * numbers are known.  Then the nth missing number is found by a linear
 * scan of the marker array. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but never used */

    static uint8_t *mark      = NULL;
    static size_t  mark_size  = 0;
    static idx_t   cur_limit  = 0;
    static idx_t   total_missing = 0;

    /* Grow the limit until we have enough missing numbers. */
    while (1) {
        if (cur_limit == 0) cur_limit = 1;                /* initial limit */

        /* Ensure the marker array is large enough. */
        if (mark_size < (size_t)(cur_limit + 1)) {
            uint8_t *tmp = (uint8_t *)realloc(mark, (size_t)(cur_limit + 1));
            if (!tmp) abort();
            mark = tmp;
            mark_size = (size_t)(cur_limit + 1);
        }

        /* Build marks for the current limit and obtain missing count. */
        total_missing = build_mark(cur_limit, mark);

        if (total_missing >= n) break;          /* enough numbers known */
        /* Double the limit for the next iteration, guarding against overflow. */
        if (cur_limit > ~(idx_t)0 >> 1) cur_limit = ~(idx_t)0;
        else cur_limit <<= 1;
    }

    /* Scan to locate the nth missing number. */
    idx_t counter = 0;
    for (idx_t i = 1; i <= cur_limit; ++i) {
        if (!mark[i]) {
            ++counter;
            if (counter == n) return (seq_t)i;
        }
    }

    /* Should never reach here for n >= 1. */
    return (seq_t)0;
}
