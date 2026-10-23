/*
 * A174546  Triangle read by rows: T(n,0) = T(n,n) = 1, T(n,k) =
 *          Stirling2(n,m) * 3^m where m = min(k,n-k).
 * https://oeis.org/A174546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = Stirling2(n,min(k,n-k)) *
 *            3^min(k,n-k) * and r is the unique integer with r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2. The * function returns the sequence term a(n) for
 *            all n ≥ 0
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Integer square root of a 128‑bit value ---------- */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);   /* > sqrt(max) */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        /* mid*mid <= x  →  x/mid >= mid   (avoids 128‑bit overflow) */
        if (x / mid >= mid)
            low = mid;
        else
            high = mid - 1;
    }
    return (idx_t)low;
}

/* ---------- Storage for Stirling numbers of the second kind ---------- */
static seq_t **S = NULL;        /* S[n][k] = Stirling2(n,k)          */
static idx_t max_row = 0;       /* highest n for which a row exists */

/* Ensure Stirling numbers for rows 0..up_to are computed. */
static void ensure_stirling(idx_t up_to)
{
    if (up_to <= max_row)
        return;

    S = (seq_t **)realloc(S, (up_to + 1) * sizeof(seq_t *));
    if (S == NULL)
        abort();

    for (idx_t i = max_row + 1; i <= up_to; ++i)
        S[i] = NULL;

    for (idx_t n = max_row + 1; n <= up_to; ++n) {
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (row == NULL)
            abort();

        if (n == 0) {
            row[0] = 1;
        } else {
            row[0]      = 0;
            row[n]      = 1;
            for (idx_t k = 1; k < n; ++k) {
                seq_t tmp = S[n - 1][k];
                tmp = tmp * (seq_t)k;
                row[k] = tmp + S[n - 1][k - 1];
            }
        }
        S[n] = row;
    }
    max_row = up_to;
}

/* ---------- Power of 3 (sequence type independent) ---------- */
static seq_t pow3_u(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * (seq_t)3;
    return r;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Locate the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t sqrt_val = isqrt_uint128((unsigned __int128)8 * n + 1);
    idx_t r = (sqrt_val - 1) / 2;

    /* Ensure Stirling numbers for row r are available */
    ensure_stirling(r);

    /* Position of n inside row r :  k = n - T(r) */
    seq_t tri = (seq_t)r * (r + 1) / 2;          /* triangular number T(r) */
    idx_t k = n - (idx_t)tri;

    /* Boundary values are 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    idx_t m = (k < r - k) ? k : (r - k);

    seq_t stirling = S[r][m];
    seq_t threes   = pow3_u(m);
    return stirling * threes;
}
