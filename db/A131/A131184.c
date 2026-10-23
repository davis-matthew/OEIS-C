/*
 * A131184  Table T(n,m): the length of the sequence b(1)=n, b(2)=m, b(i) =
 *          b(i-1)*b(i-2) mod 10 up to the end of b's first cycle.
 * https://oeis.org/A131184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, tabf, nonn, less
 * Formula:   a(n) = T( ((n-1)/9)+1, ((n-1)%9)+1 ), where T(p,q) is the number
 *            of terms produced by the sequence b(1)=p, b(2)=q,
 *            b(i)=b(i-1)*b(i-2) mod 10 until the first pair (b(i-1),b(i))
 *            repeats. The table T(p,q) depends only on the last decimal digit
 *            of p and q. The function precomputes T for all 10×10 pai...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   init_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/init_T.h"

seq_t a(idx_t n)
{
    static seq_t T[10][10];
    static bool initialized = false;
    if (!initialized) {
        init_T(T);
        initialized = true;
    }

    /* Map index n to a pair (p,q) as described:
       p = ((n-1)/9)+1,  q = ((n-1)%9)+1
       Only the last decimal digit of p and q matters.   */
    idx_t r = (n - 1) / 9;
    idx_t c = (n - 1) % 9;
    int pd = (int)((r + 1) % 10);   /* last digit of p */
    int qd = (int)((c + 1) % 10);   /* last digit of q (1..9) */

    return T[pd][qd];
}
