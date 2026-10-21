/*
 * A223512  Triangle T(n,k) represents the coefficients of (x^10*d/dx)^n,
 *          where n=1,2,3,...;generalization of Stirling numbers of second
 *          kind A008277, Lah-numbers A008297.
 * https://oeis.org/A223512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(1,1)=1; for 1≤k≤n, T(n,k)= (9*n + k - 9)*T(n-1,k)+T(n-1,k-1).
   a(n) is the n-th term of the triangle written row‑wise:
   let r be the smallest r with r(r+1)/2 ≥ n, and s = n - (r-1)r/2.
   Then a(n) = T(r,s). */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for offset 1, but safe guard */

    /* compute row r such that r(r+1)/2 >= n  */
    idx_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n ) ++r;

    idx_t s = n - ((r - 1) * r) / 2;      /* position in the row (1‑based) */

    /* buffers for DP, size 200 > maximal r for n up to 10000 */
    static seq_t prev[200];
    static seq_t curr[200];
    for (idx_t i = 0; i <= r; ++i) prev[i] = curr[i] = 0;
    prev[1] = 1;                         /* T(1,1)=1 */

    for (idx_t level = 2; level <= r; ++level) {
        /* compute new row level */
        for (idx_t k = 1; k <= level; ++k) {
            seq_t factor = (seq_t)9 * level + (seq_t)k - 9;   /* (9n+k-9) */
            seq_t term1 = prev[k] * factor;                  /* (9n+k-9)*T(n-1,k) */
            seq_t term2 = (k > 1) ? prev[k - 1] : 0;         /* T(n-1,k-1) */
            curr[k] = term1 + term2;
        }
        /* swap rows: curr -> prev, clear curr for next iteration */
        for (idx_t k = 1; k <= level; ++k) {
            prev[k] = curr[k];
            curr[k] = 0;
        }
    }

    return prev[s];
}
