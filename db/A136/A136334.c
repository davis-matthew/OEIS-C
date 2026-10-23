/*
 * A136334  Triangular sequence from both a cubic expansion polynomial and a
 *          three deep polynomial recursion: Expansion polynomial: f(x,t)=1/(1
 *          - 2*x*t + t^3); Recursion polynomials: p(x, n) = 2*x*p(x, n - 1) -
 *          p(x, n - 3);.
 * https://oeis.org/A136334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence is the row‑by‑row listing of the coefficients of the
 * polynomials p_k(x) defined by 
 *   p_0(x) = 1,
 *   p_1(x) = 2x,
 *   p_2(x) = 4x²,
 *   p_k(x) = 2*x*p_{k-1}(x) – p_{k-3}(x)   for k ≥ 3.
 * 
 * The triangular array has row k consisting of the coefficients of p_k(x)
 * in increasing powers of x (constant term first).  The OEIS terms a(n)
 * are obtained by concatenating the rows.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t
find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;              /* high is always large enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* cumulative terms up to row mid: (mid+1)(mid+2)/2 */
        idx_t cum = (idx_t)(((__int128)(mid + 1) * (mid + 2)) / 2);
        if (cum < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;                  /* minimal r with cum >= n */
}

seq_t a(idx_t n)
{
    /* n >= 1 by the offset definition */
    idx_t r = find_row(n);                   /* row index */
    /* terms before row r */
    idx_t before = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t pos = n - before - 1;              /* 0‑based position in row r */

    /* arrays for the last three rows */
    seq_t *poly[3] = {NULL, NULL, NULL};
    idx_t len[3] = {0, 0, 0};

    /* base rows */
    poly[0] = (seq_t *)malloc(sizeof(seq_t));
    poly[0][0] = (seq_t)1;
    len[0] = 1;

    if (r == 0)
        return poly[0][pos];

    poly[1] = (seq_t *)malloc(2 * sizeof(seq_t));
    poly[1][0] = (seq_t)0;
    poly[1][1] = (seq_t)2;
    len[1] = 2;

    if (r == 1)
        return poly[1][pos];

    poly[2] = (seq_t *)malloc(3 * sizeof(seq_t));
    poly[2][0] = (seq_t)0;
    poly[2][1] = (seq_t)0;
    poly[2][2] = (seq_t)4;
    len[2] = 3;

    if (r == 2)
        return poly[2][pos];

    /* Compute successive rows up to r */
    for (idx_t k = 3; k <= r; ++k) {
        idx_t idx = k % 3;
        idx_t prev1 = (k - 1) % 3;
        idx_t prev3 = (k - 3) % 3;
        idx_t new_len = k + 1;

        seq_t *cur = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cur) {
            /* memory failure – free everything and return 0 */
            for (idx_t i = 0; i < 3; ++i) free(poly[i]);
            return (seq_t)0;
        }

        for (idx_t i = 0; i < new_len; ++i) {
            seq_t term1 = (i >= 1 && i-1 < len[prev1]) ? (seq_t)(2 * poly[prev1][i - 1]) : (seq_t)0;
            seq_t term2 = (i + 3 <= k && i < len[prev3]) ? poly[prev3][i] : (seq_t)0;
            cur[i] = term1 - term2;
        }

        free(poly[idx]);          /* discard the row that is no longer needed */
        poly[idx] = cur;
        len[idx] = new_len;
    }

    seq_t result = poly[r % 3][pos];

    /* clean up all allocated rows */
    for (idx_t i = 0; i < 3; ++i) free(poly[i]);

    return result;
}
