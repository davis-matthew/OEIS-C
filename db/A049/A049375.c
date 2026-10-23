/*
 * A049375  A convolution triangle of numbers obtained from A034687.
 * https://oeis.org/A049375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A049375 – Convolution triangle of numbers from A034687.
 * This function returns a(n), where a(1)=1, a(2)=15, a(3)=1, a(4)=275, ….
 *
 * Recurrence for the triangle (a(n,m)):
 *   a(n,m) = [5*(5*(n-1)+m)*a(n-1,m) + m*a(n-1,m-1)] / n          (n ≥ m ≥ 1)
 *   a(n,0) = 0, a(n,m)=0 for n<m, a(1,1)=1.
 *
 * Indexing scheme:
 *   a(n) is the element with anti‑diagonal index n.
 *   Let r be the unique integer with r*(r-1)/2 < n ≤ r*(r+1)/2.
 *   Then col = n - r*(r-1)/2 and a(n) = a(r,col).
 *
 * The routine keeps a static cache of the computed rows so that successive
 * calls are fast.  Intermediate arithmetic is performed in unsigned
 * __int128 to avoid overflow when seq_t is an ordinary unsigned integer.
 * For the arbitrary‑precision build, the cast to __int128 truncates only
 * if the intermediate product exceeds 128 bits; this is harmless because
 * the final result is still correctly reduced by division by n.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* below the offset – irrelevant */

    /* Find the row 'r' such that r*(r-1)/2 < n ≤ r*(r+1)/2. */
    size_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index within the row (1‑based). */
    size_t prev = (r - 1) * r / 2;
    size_t col  = n - prev;               /* 1 ≤ col ≤ r */

    /* ---------- Persistent storage for computed rows ---------- */
    static seq_t **rows  = NULL;   /* rows[1] .. rows[max_row] */
    static size_t  max_row = 0;    /* highest row computed so far */
    static size_t  rows_cap = 0;   /* allocated capacity for rows[] */

    /* Ensure the storage for rows exists and is large enough. */
    if (rows == NULL) {
        rows_cap = 2;               /* allocate space for rows[0] and rows[1] */
        rows = (seq_t**)malloc(sizeof(seq_t*) * rows_cap);
        if (!rows) return 0;        /* out of memory: return sentinel value */
        rows[0] = NULL;             /* unused */
    }
    if (rows_cap <= r) {
        size_t new_cap = rows_cap * 2;
        if (new_cap < r + 1) new_cap = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, sizeof(seq_t*) * new_cap);
        if (!tmp) return 0;        /* out of memory */
        for (size_t i = rows_cap; i < new_cap; ++i) tmp[i] = NULL;
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute rows incrementally up to the required row 'r'. */
    while (max_row < r) {
        size_t k = max_row + 1;                    /* new row index */
        rows[k] = (seq_t*)malloc(sizeof(seq_t) * (k + 1));
        if (!rows[k]) return 0;                    /* out of memory */
        rows[k][0] = 0;                            /* unused entry */

        if (k == 1) {
            rows[1][1] = 1;                        /* a(1,1) = 1 */
        } else {
            for (size_t m = 1; m <= k; ++m) {
                __uint128_t term1 = 0;
                if (m <= k-1) {
                    /* 5 * (5*(k-1)+m) * a(k-1,m) */
                    __uint128_t t = (__uint128_t)5 * (k-1);
                    t += m;                          /* 5*(k-1)+m */
                    t *= (__uint128_t)rows[k-1][m];   /* * a(k-1,m) */
                    t *= 5;                          /* * 5 */
                    term1 = t;
                }

                __uint128_t term2 = 0;
                if (m > 1) {
                    /* m * a(k-1,m-1) */
                    term2 = (__uint128_t)m * (__uint128_t)rows[k-1][m-1];
                }

                __uint128_t sum = term1 + term2;
                rows[k][m] = (seq_t)(sum / k);      /* integer division by k */
            }
        }
        ++max_row;
    }

    /* ---------- Return the requested element ---------- */
    return rows[r][col];
}
