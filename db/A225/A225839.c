/*
 * A225839  Triangular numbers representable as triangular(m) +
 *          triangular(2m).
 * https://oeis.org/A225839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   advance_once
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225839: triangular numbers T_k that can be written as
 * T_m + T_{2m}.  For each non‑negative m let
 *   x = 10*m + 3
 * and x, y satisfy the Pell equation x² – 5*y² = 4.  Only the
 * solutions with x ≡ 3 (mod 10) produce admissible m.  The
 * corresponding triangular number is
 *   a = (5*m² + 3*m)/2 .
 * This implementation generates the solutions of the Pell
 * equation with the recurrence
 *   x' = 9*x + 20*y
 *   y' = 4*x + 9*y
 * and keeps two independent streams (starting from (3,1) and
 * (7,3)).  At each step the stream whose current x is
 * smaller is chosen, its m (and thus a) is emitted, and the
 * stream is advanced to the next valid solution.
 * Results are cached in a dynamic array so that repeated
 * calls to a(n) are fast and work for any order of n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/advance_once.h"

static seq_t *cache = NULL;      /* a(1) … a(cache_size)   */
static size_t cache_size = 0, cache_cap = 0;

/* Two independent streams of Pell solutions */
static seq_t s1x, s1y;   /* corresponds to the (3,1) branch      */
static seq_t s2x, s2y;   /* corresponds to the (7,3) branch      */
static bool init_done = false;

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1, so n >= 1 is assumed. */
    if (n == 0)
        return 0;

    /* Initialise once ---------------------------------------------------- */
    if (!init_done) {
        init_done = true;

        /* First stream starting from (3,1) – already satisfies x%10==3 */
        s1x = 3; s1y = 1;

        /* Second stream starting from (7,3) – advance until x%10==3 */
        s2x = 7; s2y = 3;
        while (s2x % 10 != 3) {
            advance_once(&s2x, &s2y);
        }

        /* Initialise cache with a small capacity */
        cache_cap = 32;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
    }

    /* Generate terms until we have at least n values --------------------- */
    while (cache_size < (size_t)n) {
        seq_t x, y, m, term;

        /* Pick the stream with the smaller current x */
        if (s1x <= s2x) {
            x = s1x; y = s1y;
            /* Advance stream 1 to its next valid solution afterwards */
            do {
                advance_once(&s1x, &s1y);
            } while (s1x % 10 != 3);
        } else {
            x = s2x; y = s2y;
            /* Advance stream 2 to its next valid solution afterwards */
            do {
                advance_once(&s2x, &s2y);
            } while (s2x % 10 != 3);
        }

        /* m = (x-3)/10, then a = (5*m*m + 3*m)/2 */
        m = (x - 3) / 10;
        term = ((seq_t)5 * m * m + (seq_t)3 * m) / 2;

        /* Append the term to the cache */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            cache = tmp;
        }
        cache[cache_size++] = term;
    }

    return cache[n - 1];
}
