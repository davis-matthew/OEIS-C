/*
 * A108446  Triangle read by rows: T(n,k) is number of paths from (0,0) to
 *          (3n,0) that stay in the first quadrant (but may touch the
 *          horizontal axis), consisting of steps u=(2,1), U=(1,2), or
 *          d=(1,-1) and have k peaks of the form ud.
 * https://oeis.org/A108446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/n) * binom(n,k) * * Σ_{j=0}^{n-k} binom(n-k,j) *
 *            binom(n+2j, k+j-1) * a(n) = flattened row‑by‑row read of T(n,k)
 *            (0‑based indices)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t, idx_t) */

/* ----- find the row r such that r(r+1)/2 <= idx < (r+1)(r+2)/2 ----- */
static idx_t find_row(idx_t idx)
{
    if (idx == 0) return 0;
    idx_t low = 0, high = 1;
    /* increase high until it exceeds the row bound */
    while (((unsigned __int128)high * (high + 1)) / 2 <= idx)
        high <<= 1;
    low = high >> 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= idx)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* ----- compute triangle entry T(n,k) (0‑based) (n>0) ----- */
static seq_t triangle_T(idx_t n, idx_t k)
{
    if (n == 0)            /* T(0,0) = 1 */
        return (seq_t)1;

    seq_t bin_nk = binom(n, k);   /* binom(n,k) fits in seq_t */
    unsigned __int128 sum = 0;

    for (idx_t j = 0; j <= n - k; ++j) {
        seq_t term = binom(n - k, j) * binom(n + 2 * j, k + j - 1);
        sum += (unsigned __int128)term;
    }

    unsigned __int128 prod = (unsigned __int128)bin_nk * sum;
    prod /= (unsigned __int128)n;   /* exact division */

    return (seq_t)prod;
}

/* ----- public request routine ----- */
seq_t a(idx_t idx)
{
    idx_t row = find_row(idx);
    idx_t col = idx - row * (row + 1) / 2;
    return triangle_T(row, col);
}
