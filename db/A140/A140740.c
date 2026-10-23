/*
 * A140740  Triangle read by rows: T(n,n) = 1 and for k with 1 <= k < n:
 *          T(n+1,k) = T(n,k) + T(n - n mod k, k).
 * https://oeis.org/A140740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle T(n,k) defined by T(1,1) =
 *            1, T(n,n) = 1, and for 1 ≤ k < n: T(n+1,k) = T(n,k) + T(n - n%k
 *            , k). The sequence is read row by row, so a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(2,2), etc
 * Range:     u8 n <= 36, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Small static triangle buffer (enough for n up to about 10000) */
    enum { MaxRows = 200 };               /* > sqrt(2*10000) ≈ 142 */
    static seq_t T[MaxRows + 1][MaxRows + 1];
    static bool initialized = false;
    static idx_t computed_rows = 0;        /* rows produced so far */

    /* initialise the first cell */
    if (!initialized) {
        T[1][1] = (seq_t)1;
        computed_rows = 1;
        initialized  = true;
    }

    /* determine the row that contains the n‑th term */
    idx_t target_row = 0;
    for (idx_t r = 1; ; ++r) {
        if (r * (r + 1) / 2 >= n) {
            target_row = r;
            break;
        }
    }

    /* generate rows up to target_row */
    while (computed_rows < target_row) {
        idx_t r = computed_rows + 1;              /* next row to compute */

        /* diagonal entry */
        T[r][r] = (seq_t)1;

        /* off‑diagonal entries */
        for (idx_t k = 1; k < r; ++k) {
            seq_t t1 = T[r - 1][k];
            idx_t sub = (r - 1) - ((r - 1) % k);
            seq_t t2 = T[sub][k];
            T[r][k] = t1 + t2;
        }

        ++computed_rows;
    }

    /* find the column within that row */
    idx_t cumulative_before = (target_row - 1) * target_row / 2;
    idx_t col = n - cumulative_before;          /* 1‑based column index */

    return T[target_row][col];
}
