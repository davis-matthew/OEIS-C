/*
 * A101817  Triangle read by rows: T(n,h) = number of functions
 *          f:{1,2,...,n}->{1,2,...,n} such that |Image(f)|=h; h=1,2,...,n,
 *          n=1,2,3,... . Essentially A090657, but without zeros.
 * https://oeis.org/A101817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,h) where r is the smallest integer with r(r+1)/2 ≥ n,
 *            h = n – r(r−1)/2, and T(r,h) = C(r,h) * h! * S(r,h). S(r,h) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"

/* Dynamic table for Stirling numbers: S[n][k] = S(n,k) */
static seq_t **S = NULL;          /* rows of Stirling numbers */
static idx_t S_rows = 0;          /* number of rows currently stored */

/* Extend the Stirling table up to row n (inclusive). */
static void ensure_stirling_up_to(idx_t n)
{
    if (S_rows > n) return;      /* already enough rows */

    /* Allocate/resize row pointer array */
    idx_t old_rows = S_rows;
    S_rows = n + 1;
    S = (seq_t **)realloc(S, S_rows * sizeof(seq_t *));
    if (!S) exit(1);             /* allocation failed */

    /* Allocate new rows and initialise them */
    for (idx_t i = old_rows; i < S_rows; ++i) {
        S[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!S[i]) exit(1);
        for (idx_t j = 0; j <= i; ++j) {
            if (i == 0 && j == 0)
                S[i][j] = (seq_t)1;
            else if (j == 0)
                S[i][j] = (seq_t)0;
            else if (j == i)
                S[i][j] = (seq_t)1;
            else
                S[i][j] = (seq_t)0;    /* placeholder for recurrence */
        }
    }

    /* Fill in the newly allocated rows using the recurrence */
    for (idx_t i = old_rows; i < S_rows; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            if (j == i) continue;        /* already set to 1 */
            seq_t part1 = S[i - 1][j];
            part1 *= (seq_t)j;
            seq_t part2 = S[i - 1][j - 1];
            S[i][j] = part1 + part2;
        }
    }
}

/* Sequence a(n) for OEIS A101817. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column h within that row (1‑based). */
    idx_t prev_sum = (r * (r - 1)) / 2;
    idx_t h = n - prev_sum;

    /* Make sure Stirling numbers up to row r are available. */
    ensure_stirling_up_to(r);

    /* Compute factorial of h. */
    seq_t fact = 1;
    for (idx_t i = 1; i <= h; ++i)
        fact *= (seq_t)i;

    /* Binomial coefficient C(r,h). */
    seq_t bin = binom(r, h);

    /* Stirling number S(r,h). */
    seq_t stir = S[r][h];

    /* Result: T(r,h) = C(r,h) * h! * S(r,h). */
    return bin * fact * stir;
}
