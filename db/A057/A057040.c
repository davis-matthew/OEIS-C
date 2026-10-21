/*
 * A057040  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057040(n)=i(F(n)), where F(n) is
 *          the n-th Fibonacci number.
 * https://oeis.org/A057040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(F(n)) where F(n) is the n‑th Fibonacci number (F(1)=1,
 *            F(2)=1), and i(k) is the row index of k in the antidiagonal
 *            enumeration 1; 2,3; 4,5,6; … For k the diagonal index t =
 *            ceil((sqrt(8k+1)-1)/2). Then i(k) = k - (t-1)*t/2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   fib_pair, floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* Fibonacci pair: fn = F(n), fn1 = F(n+1) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    /* k = F(n) */
    seq_t k = fn;

    /* tmp = 8*k + 1 */
    seq_t tmp = k * (seq_t)8 + (seq_t)1;

    /* sq = floor(sqrt(tmp)) */
    seq_t sq = floor_sqrt_seq(tmp);

    /* t = floor((sq - 1) / 2)  (initial estimate) */
    seq_t t = (sq - (seq_t)1) / (seq_t)2;

    /* Ensure t satisfies t*(t+1)/2 >= k */
    seq_t maxk = t * (t + (seq_t)1) / (seq_t)2;
    if (maxk < k)
        t = t + (seq_t)1;

    /* Ensure t is not too large: (t-1)*t/2 < k */
    seq_t mink = (t - (seq_t)1) * t / (seq_t)2;
    if (mink >= k) {
        /* unlikely but just in case */
        while (mink >= k) {
            t = t - (seq_t)1;
            mink = (t - (seq_t)1) * t / (seq_t)2;
        }
    }

    /* i = k - (t-1)*t/2 */
    seq_t i = k - mink;

    return i;
}
