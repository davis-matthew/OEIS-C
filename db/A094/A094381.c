/*
 * A094381  Number of numbers having exactly n representations as ab+ac+bc
 *          with 1 <= a <= b <= c.
 * https://oeis.org/A094381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094381: number of integers N ≤ 250000 having exactly n representations
 * as a*b + a*c + b*c with 1 ≤ a ≤ b ≤ c.  The function builds a
 * histogram once and serves queries in constant time. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool ready = false;          /* are we prepared? */
static seq_t *hist = NULL;          /* histogram of representation counts */
static size_t hist_cap = 0;         /* size of hist array */

/* ------------------------------------------------------------------ */
/* Build the histogram once. This is the expensive part of the
 * computation.  The algorithm enumerates all triples a ≤ b ≤ c that
 * satisfy ab + ac + bc ≤ 250000 and counts how many times each N
 * occurs.  Afterwards it builds a histogram of those counts.      */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* ---------- enumerate all triples -------------------------------- */
    /* constant upper bound for N */
    #define MAXN 250000

    /* array of size MAXN+1 to hold the number of representations for
     * each N.  MAXN is a compile-time constant so this static array
     * is legal in C.  We use a larger type (uint32_t) to avoid overflow
     * during the counting.  The final histogram will be stored in
     * seq_t, which may be a larger type depending on the build.       */
    static uint32_t counts[MAXN + 1] = {0};

    for (idx_t a = 1; a * a <= MAXN; ++a) {
        for (idx_t b = a; ; ++b) {
            /* minimum N for this (a,b) is obtained at c = b:
             * Nmin = ab + (a+b)*b = 2*a*b + b*b.                 */
            idx_t ab   = a * b;
            idx_t Nmin = ab + (a + b) * b;
            if (Nmin > MAXN) break;          /* no larger b will work   */

            idx_t denom = a + b;
            /* maximum c such that N <= MAXN: ab + denom*c <= MAXN */
            idx_t cmax = (MAXN - ab) / denom;
            if (cmax < b) continue;          /* no valid c for this b */

            /* enumerate c from b up to cmax */
            for (idx_t c = b; c <= cmax; ++c) {
                idx_t N = ab + denom * c;
                /* N is guaranteed ≤ MAXN */
                ++counts[N];
            }
        }
    }

    /* ---------- build histogram of representation counts ------------- */
    /* find maximum representation count to size histogram appropriately */
    uint32_t maxR = 0;
    for (idx_t i = 1; i <= MAXN; ++i) {
        if (counts[i] > maxR) maxR = counts[i];
    }

    hist_cap = (size_t)maxR + 1;                  /* +1 for 0 count */
    hist = (seq_t *)calloc(hist_cap, sizeof(seq_t));
    if (hist == NULL) exit(EXIT_FAILURE);        /* should not happen */

    /* fill histogram */
    for (idx_t i = 1; i <= MAXN; ++i) {
        /* counts[i] fits into hist_cap-1 by construction */
        hist[(size_t)counts[i]]++;
    }

    ready = true;                                 /* ready for queries */
}

/* ------------------------------------------------------------------ */
/* Return the number of integers N ≤ 250000 having exactly n
 * representations.  Calls to a() are independent of the order of
 * n, but the initialization is performed only once.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!ready) init();
    if ((size_t)n >= hist_cap) return (seq_t)0;
    return hist[(size_t)n];
}
