/*
 * A194844  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=sqrt(5).
 * https://oeis.org/A194844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the element k in the permutation of (1…r) that is in
 *            position n of the triangular array, where r is the row
 *            containing n and the permutation is obtained by sorting k=1…r
 *            according to the fractional parts {k*sqrt(5)}. The function is
 *            implemented by sorting these fractional parts with a high...
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t Q = 1000000000ULL;        /* denominator, must be > max r used */
static const seq_t P = 2236067977ULL;        /* floor(sqrt(5)*Q) ≈ 2.236067977*10^9 */

static seq_t perm_sorted_idx(idx_t r, idx_t pos)
{
    /* Vector of (index, key) where key = (k*P) % Q */
    typedef struct {
        idx_t   idx;
        seq_t   key;
    }pair_t;
    pair_t arr[200];   /* r never exceeds about 150 for n≤10000 */

    for (idx_t k = 1; k <= r; ++k) {
        /* Compute (k*P) % Q with 128‑bit intermediate to avoid overflow */
        seq_t prod = (seq_t)k * P;
        arr[k-1].idx = k;
        arr[k-1].key = prod % Q;
    }

    /* Simple insertion sort – rows are small (≤~150), so O(r^2) is fine */
    for (idx_t i = 1; i < r; ++i) {
        pair_t tmp = arr[i];
        idx_t   j = i;
        while (j > 0 && arr[j-1].key > tmp.key) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = tmp;
    }

    return (seq_t)arr[pos].idx;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* index 0 is outside the offset */

    /* Find r such that r(r-1)/2 < n ≤ r(r+1)/2  (rows are 1‑based) */
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative count up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position within the row (0‑based) */
    idx_t start = cum - r;         /* index of first element in this row */
    idx_t pos   = n - start - 1;

    return perm_sorted_idx(r, pos);
}
