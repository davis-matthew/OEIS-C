/*
 * A164848  a(n) = A026741(n)/A051712(n+1).
 * https://oeis.org/A164848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = gcd(12, n/gcd(2, n))
 * Range:     u8 n <= 458, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Convert the index to seq_t and determine the factor g2 */
    seq_t n_seq = (seq_t)n;
    seq_t g2 = (n & 1) ? (seq_t)1 : (seq_t)2;   /* g2 = 1 if n is odd, else 2 */
    seq_t t = n_seq / g2;                       /* t = n / g2 */
    return gcd(t, (seq_t)12);                   /* result = gcd(12, t) */
}
