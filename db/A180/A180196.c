/*
 * A180196  Triangle read by rows: T(n,k) is the number of permutations of [n]
 *          that have k isolated entries (0 <= k <= n).
 * https://oeis.org/A180196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=k+1..floor((n+k)/2)} binom(n-1-j, j-k-1) *
 *            binom(j,k) * (d(j) + d(j-1)) if k < n T(n,n) = d(n) + d(n-1)
 *            (d(i) = derangement number) a(idx) = T(row,col) where row is the
 *            largest r with r*(r+1)/2 ≤ idx and col = idx - r*(r+1)/2.
 *            (a(idx) is the linear index of the triangle read by ro...
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *der = NULL;     /* derangements array */
static idx_t der_cap = 0;     /* capacity of der */
static idx_t der_len = 0;     /* number of filled elements */

/* Ensure der[i] is known for all i ≤ n */
static void ensure_derangements(idx_t n)
{
    if (n < der_len) return;

    idx_t new_cap = der_cap ? der_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(der, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – should not happen for n ≤ 10000 */
        return;
    }
    der = tmp;
    der_cap = new_cap;

    for (idx_t i = der_len; i <= n; ++i) {
        if (i == 0) der[i] = 1;
        else if (i == 1) der[i] = 0;
        else der[i] = (seq_t)(i - 1) * (der[i - 1] + der[i - 2]);
    }
    der_len = n + 1;
}

/* Find the row r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2 */
static idx_t row_from_index(idx_t idx)
{
    idx_t low = 0, high = idx;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t off = mid * (mid + 1) / 2;
        if (off <= idx)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = row_from_index(n);
    idx_t off = r * (r + 1) / 2;
    idx_t k = n - off;                /* column within the row */

    if (r == 0)  /* T(0,0) = 1 */
        return 1;

    ensure_derangements(r);

    if (k == r)                       /* T(n,n) case */
        return der[r] + der[r - 1];

    seq_t sum = 0;
    idx_t j_start = k + 1;
    idx_t j_end   = (r + k) / 2;       /* floor((n+k)/2) */

    for (idx_t j = j_start; j <= j_end; ++j) {
        idx_t a1 = r - 1 - j;          /* n-1-j */
        idx_t b1 = j - k - 1;          /* j-k-1 */

        seq_t t1 = binom(a1, b1);
        if (!t1) continue;
        seq_t t2 = binom(j, k);
        if (!t2) continue;
        seq_t t3 = der[j] + der[j - 1];

        seq_t term = t1 * t2;
        term *= t3;
        sum += term;
    }
    return sum;
}
