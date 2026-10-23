/*
 * A194871  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=sqrt(6).
 * https://oeis.org/A194871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry in the triangular array where * each row r
 *            consists of a permutation of (1,2,…,r) obtained by * ordering
 *            the fractional parts of r, 2r, …, rr, * with r = sqrt(6). a(1)
 *            is the first term of the first row
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/* Find a(n) for any n >= 1.  The routine uses only integer arithmetic
 * for the index logic and a straightforward O(r^2) algorithm to generate
 * the r‑th row permutation, where r ≈ sqrt(2n) (≈ 140 for n = 10 000). */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
     * Determine the row r that contains index n.
     * offset_row(r) = r*(r-1)/2 + 1   (1‑based linear index).
     * We need the smallest r with offset_row(r) <= n.
     * Solve r*(r-1) <= 2(n-1)  ->  r = floor( (1 + sqrt(1+8*(n-1))) / 2 ).
     * ------------------------------------------------------------------ */
    idx_t r;
    {
        long double nd = (long double)(n - 1);
        long double rr = floorl( (1.0L + sqrtl(1.0L + 8.0L*nd)) / 2.0L );
        r = (idx_t)rr;
    }

    /* Position within the row (1‑based). */
    idx_t start = r * (r - 1) / 2 + 1;
    idx_t pos   = n - start;             /* 0‑based index inside the row */

    /* ------------------------------------------------------------------
     * Generate the permutation for row r.
     * For each k = 1…r compute frac(k*sqrt6) and perform a simple
     * selection sort to find the ascending order of fractional parts.
     * ------------------------------------------------------------------ */
    typedef struct {
        int k;
        long double frac;
        bool used;
    } Item;

    Item items[r];
    static long double sqrt6 = 0.0L;      /* cached value of sqrt(6) */
    if (sqrt6 == 0.0L) sqrt6 = sqrtl(6.0L);

    for (int i = 0; i < (int)r; ++i) {
        int k = i + 1;
        long double val = (long double)k * sqrt6;
        long double f   = val - floorl(val); // fractional part
        items[i].k     = k;
        items[i].frac  = f;
        items[i].used  = false;
    }

    /* Find the permutation entry at position 'pos' */
    int answer = 0;
    for (int idx = 0; idx <= (int)pos; ++idx) {
        int best = -1;
        long double best_frac = 0.0L;
        for (int i = 0; i < (int)r; ++i) {
            if (items[i].used) continue;
            if (best == -1 || items[i].frac < best_frac) {
                best = i;
                best_frac = items[i].frac;
            }
        }
        items[best].used = true;
        if (idx == pos) {
            answer = items[best].k;
            break;
        }
    }

    return (seq_t)answer;
}
