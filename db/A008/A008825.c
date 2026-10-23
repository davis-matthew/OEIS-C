/*
 * A008825  Expansion of (1+2*x^5+x^9)/((1-x)^2*(1-x^9)).
 * https://oeis.org/A008825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n*(2*n + 3)/9) + [(n-1) mod 9 < 2]
 * Range:     u8 n <= 10, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t term = nn * ((seq_t)2 * nn + 3);    /* n*(2*n + 3)        */
    term /= 9;                               /* integer division   */
    seq_t res = 1 + term;                    /* 1 + floor(...)     */

    /* compute (n-1) mod 9 without underflow */
    idx_t modv = (n % 9 + 8) % 9;
    if (modv < 2) {
        res += 1;                            /* Iverson bracket    */
    }
    return res;
}
