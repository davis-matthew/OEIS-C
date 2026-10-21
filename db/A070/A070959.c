/*
 * A070959  First minimum value > 0 of the form x^3-k^2 when k > n^3.
 * https://oeis.org/A070959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*(n/2)^2 + 1 if n is even; a(n) = 8*((n+1)/2)^3 -
 *            9*((n+1)/2)^2 + 6*((n+1)/2) - 1 if n is odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1, offset 1 */
    if ((n & 1U) == 0) {                    /* even: n = 2k  */
        idx_t k_idx = n >> 1;               /* k = n/2  */
        seq_t k = (seq_t)k_idx;
        return 3 * k * k + 1;
    } else {                                 /* odd: n = 2k-1 */
        idx_t k_idx = (n + 1) >> 1;          /* k = (n+1)/2 */
        seq_t k = (seq_t)k_idx;
        seq_t k2 = k * k;                   /* k^2   */
        seq_t k3 = k2 * k;                  /* k^3   */
        seq_t term = 8 * k3;                /* 8*k^3 */
        term += 6 * k;                      /* +6k   */
        term -= 9 * k2;                     /* -9*k^2*/
        term -= 1;                          /* -1    */
        return term;
    }
}
