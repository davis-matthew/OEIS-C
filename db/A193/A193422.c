/*
 * A193422  Smallest number m such that A193358(m) = n.
 * https://oeis.org/A193422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence A193422: smallest m with A193358(m)=n.
   Formula: a(1)=0, a(2)=1;
   For n>2:
     if n even:  a(n) = (n/2) * ((n/2)-1)
     if n odd :  a(n) = ((n-1)/2) * (((n-1)/2)+3) */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    if ((n & 1ULL) == 0) {          /* n is even */
        idx_t m = n >> 1;           /* m = n/2 */
        return (seq_t)m * (seq_t)(m - 1);
    } else {                        /* n is odd */
        idx_t k = (n - 1) >> 1;     /* k = (n-1)/2 */
        return (seq_t)k * (seq_t)(k + 3);
    }
}
