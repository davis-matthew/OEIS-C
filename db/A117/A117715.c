/*
 * A117715  Triangle, read by rows, T(n, k) = Fibonacci(n, k), where
 *          Fibonacci(n, x) is the Fibonacci polynomial.
 * https://oeis.org/A117715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = F_r(k) where * r = floor((sqrt(8*n+1)-1)/2) (row index,
 *            0‑based) * k = n - r*(r+1)/2 (column index within the row,
 *            0‑based) * F_0(x)=0, F_1(x)=1, * F_m(x)=x*F_{m-1}(x)+F_{m-2}(x)
 *            (Fibonacci polynomial) * * The sequence is read row‑by‑row: row
 *            r has r+1 entries for k=0..r. * * Complexity: O...
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 75, u64 n <= 168,
 *            u128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.    */
    idx_t r = (idx_t)((sqrtl((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* Correct possible rounding errors.                                */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Column index k (0‑based).                                        */
    idx_t k = n - r * (r + 1) / 2;
    seq_t dk = (seq_t)k;

    /* Compute the Fibonacci polynomial F_r(k).                         */
    if (r == 0) return 0;
    if (r == 1) return 1;

    seq_t prev2 = 0;          /* F_{m-2} */
    seq_t prev1 = 1;          /* F_{m-1} */
    seq_t cur   = 0;          /* F_m     */

    for (idx_t i = 2; i <= r; ++i) {
        cur   = dk * prev1 + prev2;  /* F_m = k*F_{m-1} + F_{m-2} */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;          /* F_r(k) */
}
