/*
 * A115080  Triangle, read by rows, where T(n,k) equals the dot product of the
 *          vector of terms in row n that are to the right of T(n,k) with the
 *          vector of terms in column k that are above T(n,k): T(n,k) =
 *          Sum_{j=0..n-k-1} T(n,j+k+1)*T(j+k,k) for n > k+1 > 0, with T(n,n)
 *          = 1 and T(n,n-1) = n (n>=1).
 * https://oeis.org/A115080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 104, u64 n <= 299,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A115080:  T(n,k)=∑(j=0..n‑k‑1) T(n,j+k+1)*T(j+k,k),
   with T(n,n)=1, T(n,n-1)=n (n≥1).  The sequence a(n) is obtained by
   reading the triangle row by row, starting with T(0,0)=1. */

#include <stdlib.h>

static seq_t *T = NULL;              /* storage of all triangle entries        */
static size_t T_cap = 0;             /* allocated number of seq_t elements      */
static idx_t   T_max_row = (idx_t)-1;/* last row that has been computed         */

static void ensure_rows(idx_t up_to)
{
    if (T_max_row == (idx_t)-1) {
        /* nothing computed yet */
    } else if (up_to <= T_max_row) {
        return;
    }

    /* determine new capacity: (up_to+1)*(up_to+2)/2 */
    __uint128_t cap128 = (__uint128_t)(up_to + 1) * (up_to + 2) / 2;
    size_t new_cap = (size_t)cap128;
    if (new_cap > T_cap) {
        seq_t *tmp = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
        T = tmp;
        T_cap = new_cap;
    }

    idx_t start = (T_max_row == (idx_t)-1) ? 0 : T_max_row + 1;
    for (idx_t n = start; n <= up_to; ++n) {
        size_t offset = (size_t)n * (n + 1) / 2;

        /* T(n,n) = 1 */
        T[offset + n] = 1;

        /* T(n,n-1) = n (for n ≥ 1) */
        if (n >= 1) {
            T[offset + n - 1] = (seq_t)n;
        }

        /* compute remaining entries k = n-2 … 0 */
        if (n >= 2) {
            for (idx_t k = n - 2; ; --k) {
                seq_t sum = 0;
                idx_t limit = n - k - 1;   /* number of terms minus 1   */
                for (idx_t j = 0; j <= limit; ++j) {
                    size_t idx_right = offset + j + k + 1;

                    idx_t row_below = j + k;
                    size_t offset_below = (size_t)row_below * (row_below + 1) / 2;
                    size_t idx_below = offset_below + k;

                    seq_t prod = T[idx_right] * T[idx_below];
                    sum += prod;
                }
                T[offset + k] = sum;
                if (k == 0) break;
            }
        }
    }
    T_max_row = up_to;
}

/* Return the n-th term of the sequence (row‑by‑row read triangle). */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((size_t)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    size_t offset = (size_t)r * (r + 1) / 2;
    idx_t k = n - offset;

    ensure_rows(r);

    return T[offset + k];
}
