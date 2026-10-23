/*
 * A160845  Number of lines through at least 2 points of a 5 X n grid of
 *          points.
 * https://oeis.org/A160845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(f(5,n,1)-f(5,n,2)) where f(m,n,k) = Σ (n-|k
 *            x|)*(m-|k y|) summed over integers x,y with |k x|<n, |k y|<m,
 *            gcd(x,y)=1
 * Range:     u8 n <= 3, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this value for any n≥0.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline int small_gcd(int a, int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    const int m = 5;          /* fixed m value from the problem */
    seq_t f1 = (seq_t)0;      /* f(5,n,1)  */
    seq_t f2 = (seq_t)0;      /* f(5,n,2)  */

    /* ---- k = 1 ---------------------------------------------------- */
    for (int abs_y = 0; abs_y <= 4; ++abs_y) {           /* |y| < m/k  -> |y| <= 4  */
        int multY = (abs_y == 0) ? 1 : 2;                 /* symmetry in y  */
        for (int abs_x = 0; abs_x < (int)n; ++abs_x) {    /* |x| < n/k  -> |x| < n  */
            if (small_gcd(abs_x, abs_y) != 1) continue;
            int multX = (abs_x == 0) ? 1 : 2;             /* symmetry in x  */
            seq_t term = (seq_t)(n - abs_x) * (seq_t)(m - abs_y);
            f1 += term * (seq_t)(multX * multY);
        }
    }

    /* ---- k = 2 ---------------------------------------------------- */
    int max_x = (int)((n - 1) / 2);   /* |x| < n/k  -> |x| < n/2 */
    for (int abs_y = 0; abs_y <= 2; ++abs_y) {   /* |y| < m/k  -> |y| < 5/2 -> |y| <=2 */
        int multY = (abs_y == 0) ? 1 : 2;
        for (int abs_x = 0; abs_x <= max_x; ++abs_x) {
            if (small_gcd(abs_x, abs_y) != 1) continue;
            int multX = (abs_x == 0) ? 1 : 2;
            seq_t term = (seq_t)(n - 2 * abs_x) * (seq_t)(m - 2 * abs_y);
            f2 += term * (seq_t)(multX * multY);
        }
    }

    /* ---- final result --------------------------------------------- */
    return (f1 - f2) / (seq_t)2;
}
