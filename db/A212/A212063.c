/*
 * A212063  Number of (w,x,y,z) with all terms in {1,...,n} and w^2<x*y*z.
 * https://oeis.org/A212063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - Σ_{s=1}^n C(s^2), where C(t) = number of triples
 *            (x,y,z) with 1≤x,y,z≤n and xyz ≤ t
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a pair‑product frequency array and a prefix sum
    to compute C(t) in O(√t) time for each needed t == s^2.
    Complexity: O(n^2) preprocessing and O(n^2) summing,
    with at most about 2·10^8 elementary operations for n = 10 000.
    Memory: a 32‑bit prefix array of size (n·n+1). */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* trivial case */

    /* M = n*n, fits in 32 bits for the problem limits (n ≤ 10 000) */
    size_t M = (size_t)n * (size_t)n;   /* maximum product index */

    /* prefix array of counts of pairs (x,y) with product == p,
       stored as 32‑bit cumulative sums */
    uint32_t *pref = (uint32_t*)calloc(M + 1, sizeof(uint32_t));
    if (!pref) return 0;   /* allocation failure */

    /* Build pair product frequencies */
    for (int x = 1; x <= (int)n; ++x) {
        for (int y = 1; y <= (int)n; ++y) {
            size_t prod = (size_t)x * (size_t)y;    /* ≤ M */
            pref[prod]++;                           /* count pair */
        }
    }

    /* Convert to cumulative counts */
    for (size_t i = 1; i <= M; ++i) pref[i] += pref[i - 1];

    /* Accumulate Σ C(s^2) over s = 1..n */
    seq_t sumC = 0;      /* running total of C(s^2) */
    unsigned long long nn = (unsigned long long)n;

    for (int s = 1; s <= (int)n; ++s) {
        unsigned long long t = (unsigned long long)s * (unsigned long long)s;  /* s^2 */

        /* Number of p with floor(t/p) ≥ n :  p ≤ t / n  */
        size_t p_limit1 = (size_t)(t / nn);
        seq_t C = 0;

        /* First part: contribution n for all these p */
        C += (seq_t)nn * (seq_t)pref[p_limit1];

        /* Second part: process remaining p with floor(t/p) < n */
        size_t p_start = p_limit1 + 1;
        while (p_start <= t) {
            unsigned long long q = t / p_start;           /* floor(t / p_start) */
            size_t p_end = (size_t)(t / q);
            if (p_end > t) p_end = (size_t)t;

            uint32_t sum_counts = pref[p_end] - pref[p_start - 1];
            C += (seq_t)q * (seq_t)sum_counts;

            p_start = p_end + 1;
        }

        sumC += C;
    }

    /* a(n) = n^4 - Σ C(s^2) */
    seq_t total = ((seq_t)nn) * ((seq_t)nn) * ((seq_t)nn) * ((seq_t)nn);
    seq_t result = total - sumC;

    free(pref);
    return result;
}
