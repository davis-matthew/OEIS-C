/*
 * A001295  Triangle in which k-th number (0<=k<=n) in n-th row (0<=n) is
 *          number of hexads in S(5,6,12) containing k given points and
 *          missing n-k given points.
 * https://oeis.org/A001295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, fini, full
 * Formula:   T(n,k) = lambda(k,n-k), where the n given points lie inside one
 *            hexad; lambda(i,0) = C(12-i,5-i)/(6-i) for i<=5, lambda(6,0) =
 *            1, lambda(i,j) = lambda(i,j-1) - lambda(i+1,j-1); rows n = 0..6
 *            (finite)
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    long long L[7][7];
    int i, j, t;
    idx_t row = 0, total = 0, col;

    if (n > 27) return 0;
    while (total + row + 1 <= n) {
        total += row + 1;
        row++;
    }
    col = n - total;

    for (i = 0; i <= 5; i++) {
        /* C(12-i, 5-i) computed multiplicatively */
        long long c = 1;
        for (t = 1; t <= 5 - i; t++)
            c = c * (12 - i - (5 - i) + t) / t;
        L[i][0] = c / (6 - i);
    }
    L[6][0] = 1;
    for (j = 1; j <= 6; j++)
        for (i = 0; i + j <= 6; i++)
            L[i][j] = L[i][j - 1] - L[i + 1][j - 1];

    return (seq_t)L[(int)col][(int)(row - col)];
}
