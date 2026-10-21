/*
 * A208245  Triangle read by rows: a(n,k) = a(n-2,k) + a(n-2,k-1).
 * https://oeis.org/A208245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained from the triangle defined by a(n,k) = a(n-2,k)
 *            + a(n-2,k-1); with a(n,1)=1, a(n,n)=1, and a(n,k)=0 for n<k or
 *            n=0. The sequence a(n) is the flattened triangle read by rows
 * Range:     u8 n <= 195, u16 n <= 675, u32 n <= 2431, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compile‑time constants. */
enum { MAX_R = 200 };          /* enough for n up to ~20 000 */

static seq_t triangle[MAX_R + 1][MAX_R + 1]; /* 1‑based indices, row 0 unused */
static idx_t computed_row = 0;               /* highest row that has been filled */

/* Return the n‑th term of A208245 (1‑based). */
seq_t a(idx_t n)
{
    /* Locate the row r that contains position n. */
    idx_t r = 1;
    while (((idx_t)r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Compute all rows up to r that have not been computed yet. */
    if (computed_row < r) {
        for (idx_t i = computed_row + 1; i <= r; ++i) {
            for (idx_t k = 1; k <= i; ++k) {
                if (k == 1 || k == i) {
                    triangle[i][k] = 1;
                } else {
                    idx_t prev = i - 2;
                    seq_t sum = 0;
                    if (prev >= k)       sum += triangle[prev][k];
                    if (prev >= k - 1)   sum += triangle[prev][k - 1];
                    triangle[i][k] = sum;
                }
            }
        }
        computed_row = r;
    }

    /* Determine column index within the row. */
    idx_t prev_cum = ((idx_t)(r - 1) * r) / 2;
    idx_t col = n - prev_cum;   /* 1‑based column */

    return triangle[r][col];
}
