/*
 * A097744  Number of ways n can be written as difference of two distinct
 *          partition numbers.
 * https://oeis.org/A097744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs i>j≥1 such that p(i)-p(j)=n where p(i) is
 *            the i‑th partition number (A000041)
 * Range:     u8 n <= 98, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; n=0 not used */

    /* maximum number of partition terms needed for n≤10000 is < 200 */
    const size_t MAX_CAP = 200;
    seq_t p[MAX_CAP];
    p[0] = (seq_t)1;                     /* partition of 0, used only for recurrence */
    size_t m = 1;                         /* number of valid entries so far (indices 0..m-1) */

    for (idx_t i = 1; ; ++i) {
        long long sum = 0;                /* use signed to handle subtraction */
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p[i - pent1];
            if (k & 1) sum += (long long)term;
            else       sum -= (long long)term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = p[i - pent2];
            if (k & 1) sum += (long long)term;
            else       sum -= (long long)term;
        }

        p[i] = (seq_t)sum;               /* store the partition number */

        /* stop when the difference to the previous term exceeds n */
        seq_t diff = p[i] - p[i - 1];
        if (diff > n) {
            m = i;                       /* p[i] itself is too big, exclude it */
            break;
        }

        if (i + 1 >= MAX_CAP) break;     /* safety against overflow */
        m = i + 1;                       /* include this term */
    }

    /* Count pairs (i,j) with 1 ≤ j < i < m and p(i)-p(j) == n */
    seq_t cnt = 0;
    for (idx_t i = 2; i < m; ++i) {
        for (idx_t j = 1; j < i; ++j) {
            seq_t d = p[i] - p[j];
            if (d == n) ++cnt;
        }
    }
    return cnt;
}
