/*
 * A144698  Triangle of 4-Eulerian numbers.
 * https://oeis.org/A144698
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(n, n‑4) * where T is the triangle of 4‑Eulerian numbers
 *            defined by * T(n,0)=1 (n≥4) * T(4,k)=0 for k≥1 * T(n,k) =
 *            (k+1)·T(n‑1,k) + (n‑k)·T(n‑1,k‑1) (n≥5, 0≤k≤n‑4) * * The routine
 *            walks the triangle row by row, keeping only two * consecutive
 *            rows in memory so that a(n) can be obtained in...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 74, u64 n <= 201,
 *            u128 n <= 545, big any n
 * Validated: standard, pass, 61 terms through n = 1328 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return the value T(n,k) defined above (n≥4, 0≤k≤n‑4). */
static seq_t tri_value(idx_t n, idx_t k)
{
    /* Length of row n is (n‑3).  Allocate two temporary rows. */
    size_t len   = (size_t)(n - 3);
    seq_t *prev  = mem_new(seq_t, len);
    seq_t *cur   = mem_new(seq_t, len);
    if (!prev || !cur) { abort(); }

    /* Initialise the first row: T(4,0)=1. */
    prev[0] = (seq_t)1;
    size_t prev_len = 1;

    /* Handle the trivial case n==4. */
    if (n == 4) {
        seq_t res = (k == 0) ? prev[0] : (seq_t)0;
        mem_free(prev); mem_free(cur);
        return res;
    }

    /* Build rows from 5 up to n.  Only two rows are kept at once. */
    for (idx_t row = 5; row <= n; ++row) {
        size_t cur_len = (size_t)(row - 3);
        for (size_t j = 0; j < cur_len; ++j) {
            seq_t t1 = (j < prev_len) ? prev[j] : (seq_t)0;
            seq_t t2 = (j > 0)         ? prev[j-1] : (seq_t)0;
            cur[j] = (seq_t)(j + 1) * t1 + (seq_t)(row - j) * t2;
        }
        /* Swap the rows for the next iteration. */
        seq_t *tmp = prev;
        prev       = cur;
        cur        = tmp;
        prev_len   = cur_len;
    }

    seq_t result = prev[k];
    mem_free(prev); mem_free(cur);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;          /* undefined for n<4 */

    /* Convert the 1‑based index n into a 0‑based position m within the
       triangle, then locate the corresponding row r and column k. */
    idx_t m = n - 4;                     /* 0‑based offset */
    idx_t r = 4;                         /* current row number   */
    idx_t cumulative = 0;                /* elements before row r */

    while (cumulative + (r - 3) <= m) {
        cumulative += (r - 3);
        ++r;
    }

    idx_t k = m - cumulative;            /* column within the row r */

    return tri_value(r, k);
}
