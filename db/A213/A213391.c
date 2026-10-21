/*
 * A213391  Number of (w,x,y) with all terms in {0,...,n} and 2*max(w,x,y) <
 *          3*min(w,x,y).
 * https://oeis.org/A213391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/9 if n % 3 == 0 * (n^3 + 6n + 2)/9 if n % 3 == 1 *
 *            (n^3 + 6n - 2)/9 if n % 3 == 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    idx_t r = n % 3u;                /* n modulo 3 */
    seq_t nt   = (seq_t)n;
    seq_t n3   = nt * nt * nt;       /* n^3 */
    seq_t six_n= (seq_t)6 * nt;      /* 6n */
    seq_t num;                       /* numerator */

    if (r == 0)          num = n3;
    else if (r == 1)     num = n3 + six_n + (seq_t)2;
    else                 num = n3 + six_n - (seq_t)2;

    return num / (seq_t)9;
}
