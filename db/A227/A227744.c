/*
 * A227744  Squares that occur in A173318.
 * https://oeis.org/A227744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A173318(A227743(n)); computed by scanning the partial
 *            sums A173318(i) = Sum_{k=0..i} A005811(k) and collecting the
 *            values that are perfect squares
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 996 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static int A227744_is_sq(uint64_t s)
{
    uint64_t r = (uint64_t)sqrt((double)s);
    while (r * r > s) r--;
    while ((r + 1) * (r + 1) <= s) r++;
    return r * r == s;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t i = 0, S = 0;
    static int started = 0;

    if (!started) {
        started = 1;
        cap = 1024;
        terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
        terms[cnt++] = 0; /* i = 0, S = 0 */
    }
    while ((idx_t)cnt < n) {
        i++;
        S += (uint64_t)__builtin_popcountll(i ^ (i >> 1));
        if (A227744_is_sq(S)) {
            if (cnt == cap) {
                cap *= 2;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = S;
        }
    }
    return (seq_t)terms[n - 1];
}
