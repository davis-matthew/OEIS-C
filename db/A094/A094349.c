/*
 * A094349  Numbers k such that A094291(k) is odd.
 * https://oeis.org/A094349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A094291(k) is odd, where A094291(k)
 *            = max_{0<=j<=i<=k} C(i,j)*C(k-j,k-i). The max is C(k-j,j)^2 or
 *            C(k-j-1,j)*C(k-j,j+1) near j ~ 0.2764k, found by scanning a
 *            window; parities come from Kummer's theorem
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static double A094349_lc(uint64_t a, uint64_t b)
{
    return lgamma((double)a + 1.0) - lgamma((double)b + 1.0) - lgamma((double)(a - b) + 1.0);
}

static bool A094349_odd_max(uint64_t k)
{
    uint64_t half = k / 2;
    uint64_t c = (uint64_t)(0.2764 * (double)k + 0.5);
    uint64_t lo = c > 6 ? c - 6 : 0;
    uint64_t hi = c + 6;
    if (hi > half) hi = half;
    if (lo > hi) lo = hi;
    double best = -1.0;
    bool bestodd = false;
    for (uint64_t j = lo; j <= hi; j++) {
        /* candidate 1: C(k-j,j)^2 */
        {
            double v = 2.0 * A094349_lc(k - j, j);
            bool od = (j & (k - 2 * j)) == 0;
            if (v > best + 1e-9) { best = v; bestodd = od; }
            else if (v > best - 1e-9 && od) { bestodd = true; if (v > best) best = v; }
        }
        /* candidate 2: C(k-j-1,j)*C(k-j,j+1), needs k >= 2j+1 */
        if (k >= 2 * j + 1) {
            double v = A094349_lc(k - j - 1, j) + A094349_lc(k - j, j + 1);
            bool od = ((j & (k - 2 * j - 1)) == 0) && (((j + 1) & (k - 2 * j - 1)) == 0);
            if (v > best + 1e-9) { best = v; bestodd = od; }
            else if (v > best - 1e-9 && od) { bestodd = true; if (v > best) best = v; }
        }
    }
    return bestodd;
}

static uint64_t *A094349_terms = NULL;
static size_t A094349_cnt = 0, A094349_cap = 0;
static uint64_t A094349_next = 1;

seq_t a(idx_t n)
{
    while (A094349_cnt < (size_t)n) {
        if (A094349_odd_max(A094349_next)) {
            if (A094349_cnt == A094349_cap) {
                A094349_cap = A094349_cap ? A094349_cap * 2 : 1024;
                A094349_terms = (uint64_t *)realloc(A094349_terms, A094349_cap * sizeof(uint64_t));
            }
            A094349_terms[A094349_cnt++] = A094349_next;
        }
        A094349_next++;
    }
    return (seq_t)A094349_terms[n - 1];
}
