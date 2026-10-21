/*
 * A216625  Triangle read by rows, n >= 1, 1 <= k <= n, T(n,k) = Sum_{c|n,d|k}
 *          gcd(c,d).
 * https://oeis.org/A216625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = Σ_{c|n, d|k} gcd(c,d) * (row,
 *            col) are obtained by reading the lower‑triangular array
 *            row‑by‑row
 * Range:     u8 n <= 665, u16 n <= 9898, u32 n <= 9898, u64 n <= 9898,
 *            u128 n <= 9898, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/*  Static storage of divisor lists                                     */
/* ------------------------------------------------------------------ */
static idx_t **divs        = NULL;   /* divs[i] -> array of all divisors of i   */
static size_t *div_counts  = NULL;   /* div_counts[i] -> number of divisors    */
static idx_t   max_rows    = 0;      /* highest n for which we have lists      */

/* ------------------------------------------------------------------ */
/*  Map linear index n to (row, col)                                   */
/* ------------------------------------------------------------------ */
static void row_col_from_index(idx_t n, idx_t *row_out, idx_t *col_out)
{
    idx_t low = 1, high = n;                     /* high is over‑estimate       */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;
    __uint128_t prev_tri = (__uint128_t)(row - 1) * row / 2;
    idx_t col = (idx_t)(n - prev_tri);           /* 1‑based column in the row   */
    *row_out = row;
    *col_out = col;
}

/* ------------------------------------------------------------------ */
/*  Ensure divisor lists exist for all integers up to `need`          */
/* ------------------------------------------------------------------ */
static void ensure_divisors(idx_t need)
{
    if (need <= max_rows)
        return;

    idx_t new_max = need;
    /* reallocate arrays to accommodate new indices */
    idx_t **new_divs = (idx_t **)realloc(divs, (new_max + 1) * sizeof(idx_t *));
    size_t *new_counts = (size_t *)realloc(div_counts, (new_max + 1) * sizeof(size_t));
    if (!new_divs || !new_counts)
        return;   /* on allocation failure, we remain partially initialized   */

    divs = new_divs;
    div_counts = new_counts;
    /* initialize index 0 */
    divs[0] = NULL;
    div_counts[0] = 0;

    for (idx_t i = max_rows + 1; i <= new_max; ++i) {
        /* count divisors first */
        size_t cnt = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                ++cnt;
                if (d * d != i)
                    ++cnt;
            }
        }
        div_counts[i] = cnt;
        idx_t *arr = (idx_t *)malloc(cnt * sizeof(idx_t));
        size_t pos = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                arr[pos++] = d;
                if (d * d != i)
                    arr[pos++] = i / d;
            }
        }
        divs[i] = arr;
    }
    max_rows = new_max;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is undefined for n=0, but we return 0 safely */

    idx_t row, col;
    row_col_from_index(n, &row, &col);

    ensure_divisors(row);   /* make sure we have divisor lists up to `row` */

    seq_t sum = 0;
    const idx_t *row_divs = divs[row];
    const idx_t *col_divs = divs[col];
    const size_t rcnt = div_counts[row];
    const size_t ccnt = div_counts[col];

    for (size_t i = 0; i < rcnt; ++i) {
        for (size_t j = 0; j < ccnt; ++j) {
            seq_t g = gcd((seq_t)row_divs[i], (seq_t)col_divs[j]);
            sum += g;
        }
    }
    return sum;
}
