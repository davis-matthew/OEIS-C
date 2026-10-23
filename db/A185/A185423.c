/*
 * A185423  Ordered forests of k increasing plane unary-binary trees with n
 *          nodes.
 * https://oeis.org/A185423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(m,k), where T(m,k)
 *            counts ordered forests of k increasing plane unary-binary trees
 *            with m nodes. The recurrence is T(m+1,k) = k * ( T(m,k-1) +
 *            T(m,k) + T(m,k+1) ), with T(1,1) = 1. a(n) is the (n)-th element
 *            of the triangle read by rows (m from 1 upwa...
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for the triangle */
    static seq_t T[200][200] = {0};
    static idx_t maxR = 0;
    static int initialized = 0;

    if (!initialized) {
        T[1][1] = (seq_t)1;
        maxR = 1;
        initialized = 1;
    }

    /* find row m such that m*(m+1)/2 >= n */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n)
        ++m;

    /* compute rows up to m if needed */
    for (idx_t r = maxR + 1; r <= m; ++r) {
        for (idx_t k = 1; k <= r; ++k) {
            seq_t sum = (seq_t)0;
            if (k > 1)
                sum += T[r - 1][k - 1];
            if (k <= r - 1)
                sum += T[r - 1][k];
            if (k + 1 <= r - 1)
                sum += T[r - 1][k + 1];
            T[r][k] = (seq_t)k * sum;
        }
    }
    maxR = m;

    /* index within the row */
    idx_t prev = (m * (m - 1)) / 2;
    idx_t k = n - prev;   /* 1 <= k <= m */
    return T[m][k];
}
