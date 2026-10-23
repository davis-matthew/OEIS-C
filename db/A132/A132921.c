/*
 * A132921  Triangle read by rows: T(n,k) = n + Fibonacci(k) - 1, 1 <= k <= n.
 * https://oeis.org/A132921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r + F(k) - 1 where r*(r-1)/2 < n ≤ r*(r+1)/2 * (row
 *            number r), k = n - r*(r-1)/2 and F(k) is the kth Fibonacci
 *            number * with F(1)=F(2)=1
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: n must be ≥ 1 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 1;
    idx_t cum = 1;          /* cumulative number of terms up to row r */

    while (cum < n) {
        r++;
        cum += r;
    }

    /* Position within the row: k = n - (r-1)*r/2 */
    idx_t k = n - (r * (r - 1) / 2);

    /* Compute Fibonacci(k) iteratively */
    seq_t fib_k;
    if (k <= 2) {
        fib_k = (seq_t)1;
    } else {
        seq_t a1 = (seq_t)1;
        seq_t a2 = (seq_t)1;
        for (idx_t i = 3; i <= k; ++i) {
            seq_t tmp = a1 + a2;
            a1 = a2;
            a2 = tmp;
        }
        fib_k = a2;
    }

    /* Return the triangle entry: r + F(k) - 1 */
    return r + fib_k - (seq_t)1;
}
