/*
 * A048160  Triangle giving T(n,k) = number of (n,k) labeled rooted Greg trees
 *          (n >= 1, 0<=k<=n-1).
 * https://oeis.org/A048160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = flattening of the triangle T(n,k) defined by
 *            T(n,0)=n^(n-1),
 *            T(n,k)=(n+k-2)*T(n-1,k-1)+(2*n+2*k-2)*T(n-1,k)+(k+1)*T(n-1,k+1)
 *            for 1≤n, 0≤k≤n-1. The sequence a(1),a(2),… is obtained by
 *            listing the rows of this triangle from left to right
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root of a 64‑bit unsigned integer */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* fast exponentiation for seq_t types */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* storage for computed rows of the triangle */
static seq_t **rows = NULL;   /* rows[1] is first row (length 1) */
static size_t rows_cap = 0;   /* largest row index computed */

/* ensure that rows[1..need] are computed and stored */
static void ensure_rows(size_t need)
{
    if (need <= rows_cap) return;

    /* enlarge pointer array */
    seq_t **new_rows = (seq_t**)realloc(rows, (need + 1) * sizeof(seq_t*));
    if (!new_rows) exit(1);
    rows = new_rows;

    /* compute missing rows */
    for (size_t r = rows_cap + 1; r <= need; ++r) {
        seq_t *row = (seq_t*)malloc(r * sizeof(seq_t));
        if (!row) exit(1);
        rows[r] = row;

        if (r == 1) {
            row[0] = 1;
        } else {
            seq_t *prev = rows[r - 1];
            /* first element */
            row[0] = pow_seq((seq_t)r, (idx_t)r - 1);

            /* remaining elements */
            for (size_t k = 1; k < r; ++k) {
                seq_t val = 0;
                seq_t term;

                /* (r + k - 2) * T(r-1, k-1) */
                term = (seq_t)(r + k - 2) * prev[k - 1];
                val += term;

                /* (2*r + 2*k - 2) * T(r-1, k) */
                if (k <= r - 2) {
                    term = (seq_t)(2 * r + 2 * k - 2) * prev[k];
                    val += term;
                }

                /* (k + 1) * T(r-1, k+1) */
                if (k + 1 <= r - 2) {
                    term = (seq_t)(k + 1) * prev[k + 1];
                    val += term;
                }

                row[k] = val;
            }
        }
    }
    rows_cap = need;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    /* find row r containing index n (1‑based) */
    idx_t r = (int_sqrt(8 * n + 1) - 1) / 2;
    while (r * (r + 1) / 2 < n) r++;    /* safety check */

    /* column k in that row */
    idx_t k = n - (r - 1) * r / 2 - 1;

    ensure_rows((size_t)r);

    return rows[r][k];
}
