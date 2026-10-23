/*
 * A194404  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=(1+sqrt(5))/2 and < > denotes fractional part.
 * https://oeis.org/A194404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194404:
   Numbers m such that
   Sum_{k=1..m} ( frac(1/2 + k*r) - frac(k*r) ) > 0,
   where r = (1+sqrt(5))/2.
   Equivalent condition: let P(m) be the number of k≤m with
   frac(k*r) < 1/2.  Then m belongs to the sequence iff 2*P(m) > m.
   The function generates the terms incrementally and caches them so
   that repeated calls (in any order) are fast. */
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of sequence values up to the largest n requested. */
    static seq_t *values = NULL;
    static size_t size   = 0;      /* number of values stored   */
    static size_t cap    = 0;      /* array capacity            */

    /* Current state of the incremental counting. */
    static idx_t checked_m = 0;    /* last k examined           */
    static idx_t less_cnt  = 0;    /* number of k with frac(k*r) < 0.5 */

    /* Golden ratio as a high‑precision long double. */
    static const long double r =
        (1.0L + sqrtl(5.0L)) / 2.0L;

    /* Grow the cache as needed.  Cast is required for C++. */
    if (size == cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable failure */
        values = tmp;
        cap    = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        ++checked_m;                                /* new k to examine */

        /* Compute fractional part of checked_m * r. */
        long double f = checked_m * r;
        f -= floorl(f);    /* 0 <= f < 1 */

        if (f < 0.5L)
            ++less_cnt;

        /* m is in the sequence if 2*less_cnt > checked_m. */
        if (2 * less_cnt > checked_m) {
            values[size++] = (seq_t)checked_m;
        }
    }

    return values[n - 1];
}
