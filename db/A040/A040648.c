/*
 * A040648  Continued fraction for sqrt(674).
 * https://oeis.org/A040648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=25; if n odd then 1; else if v2(n)=1 then 24; else 50
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)25;
    if (n & 1) return (seq_t)1;                 /* n is odd */
    /* n is even */
    idx_t e = 0;
    idx_t t = n;
    while ((t & 1) == 0) {                      /* count the 2‑adic value */
        t >>= 1;
        ++e;
    }
    if (e == 1) return (seq_t)24;
    return (seq_t)50;
}
