/*
 * A072346  Volume of n-dimensional sphere of radius r is V_n*r^n =
 *          Pi^(n/2)*r^n/(n/2)! = C_n*Pi^floor(n/2)*r^n; sequence gives
 *          denominator of C_n.
 * https://oeis.org/A072346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n/2)! if n even, else n!! (product of odd numbers up to
 *            n)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Even n: compute (n/2)! */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;          /* n / 2 */
        seq_t res = 1;
        for (idx_t i = 2; i <= k; ++i) {
            res *= (seq_t)i;
        }
        return res;
    }

    /* Odd n: compute double factorial n!! = 1 * 3 * 5 * ... * n */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; i += 2) {
        res *= (seq_t)i;
    }
    return res;
}
