/*
 * A182720  Triangle read by rows: T(n,k) = A000041(k) if k divides n,
 *          T(n,k)=0 otherwise.
 * https://oeis.org/A182720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A182720
 *  Sequence: 1, 1, 2, 1, 0, 3, 1, 2, 0, 5, …
 *  Formula:   a(n) = T(r, k) where
 *    r = minimal integer with r*(r+1)/2 >= n      (row)
 *    k = n - (r-1)*r/2                            (column)
 *    T(r, k) = A000041(k) if k divides r,
 *              0 otherwise
 *  This implementation computes the triangular decomposition
 *  and evaluates the partition number p(k) (A000041).  The
 *  partition numbers up to 200 are pre‑computed using Euler's
 *  pentagonal‑number recurrence once per program run.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

static seq_t part[201] = {0};
static bool part_computed = false;

/* compute p(0..limit) using Euler's pentagonal recurrence   */
static void compute_partitions(void)
{
    const size_t LIMIT = 200;
    if (part_computed) return;
    part[0] = (seq_t)1;            /* p(0) = 1 */

    for (size_t n = 1; n <= LIMIT; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            idx_t g1 = (idx_t)k * (3 * k - 1) / 2;
            idx_t g2 = (idx_t)k * (3 * k + 1) / 2;
            if (g1 > n && g2 > n) break;

            if (g1 <= n) {
                if ((k & 1) != 0)          /* sign = +1 */
                    sum += part[n - g1];
                else                       /* sign = -1 */
                    if (sum >= part[n - g1]) sum -= part[n - g1];
            }
            if (g2 <= n) {
                if ((k & 1) != 0)
                    sum += part[n - g2];
                else
                    if (sum >= part[n - g2]) sum -= part[n - g2];
            }
        }
        part[n] = sum;
    }
    part_computed = true;
}

/* main sequence entry point                                        */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* safety, offset is 1 */

    compute_partitions();             /* ensure partition table ready */

    /* find row r such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    idx_t tri = 1;                    /* triangular(r) */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* start index of row r */
    idx_t start = tri - r + 1;
    /* column index within the row (1‑based) */
    idx_t k = n - start + 1;

    /* return p(k) if k divides r, else 0 */
    if (k == 0) return (seq_t)0;      /* defensive */
    if (r % k == 0) {
        if (k <= 200)
            return part[(size_t)k];
        else
            return (seq_t)0;          /* should not happen for given limits */
    }
    return (seq_t)0;
}
