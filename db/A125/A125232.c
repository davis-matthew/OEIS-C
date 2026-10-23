/*
 * A125232  Triangle T(n,k) read by rows: the (n-k)-th term of the k-fold
 *          iterated partial sum of the pentagonal numbers.
 * https://oeis.org/A125232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n
 *            T(r,0) = r*(3r-1)/2 (pentagonal numbers) T(r,k) = T(r‑1,k) +
 *            T(r‑1,k‑1) for k>0 (for k = r‑1 we have T(r,r‑1) = T(r‑1,r‑2)).
 *            * The triangle is built lazily and cached; each call works in
 *            worst * case O(r) with r≈√(2n). For n≤10 000 the l...
 * Range:     u8 n <= 30, u16 n <= 141, u32 n <= 542, u64 n <= 2173,
 *            u128 n <= 8446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t T[200][200];    /* cached triangle values */
static idx_t max_row = 0;    /* rows already initialized (0…max_row) */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        max_row = 0;
    }

    /* find the row that contains the n-th term */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) {
        ++r;                       /* r is at most about 140 for n≤10 000 */
    }

    /* extend the cache up to row r (if needed) */
    if (r > max_row) {
        for (idx_t i = max_row + 1; i <= r; ++i) {
            seq_t sn = (seq_t)i;
            /* T(i,0) = pentagonal number */
            T[i - 1][0] = sn * (((seq_t)3 * sn) - (seq_t)1) / (seq_t)2;

            /* compute the rest of the row */
            for (idx_t k = 1; k < i; ++k) {
                if (k == i - 1) {
                    /* last column comes only from the previous row's
                     * last element (T(i-1,i-2)) */
                    T[i - 1][k] = T[i - 2][k - 1];
                } else {
                    T[i - 1][k] = T[i - 2][k] + T[i - 2][k - 1];
                }
            }
        }
        max_row = r;
    }

    /* index within the selected row (0‑based) */
    idx_t previous_elements = (r - 1) * r / 2;   /* #elements before row r */
    idx_t k = n - previous_elements - 1;        /* column in row r */

    return T[r - 1][k];
}
