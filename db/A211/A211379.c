/*
 * A211379  Number of pairs of parallel diagonals in a regular n-gon.
 * https://oeis.org/A211379
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even? (1/8)*n*(n-4)^2 : (1/8)*n*(n-3)*(n-5))
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 3252, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Computed exactly for all n >= 3; a(3)=a(4)=a(5)=0) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* below the OEIS offset, behaviour is unspecified; return 0 */
    if (n < 3) return 0;

    if ((n & 1) == 0) {            /* n is even */
        seq_t t1 = (seq_t)n;
        seq_t t2 = (seq_t)(n - 4); /* (n-4) is safe because n>=4 */
        seq_t prod = t1 * t2;
        prod = prod * t2;          /* t2*t2 */
        prod = prod / 8;           /* integer division is exact */
        return prod;
    } else {                       /* n is odd */
        if (n <= 5) return 0;      /* covers n=3 and n=5 where formula yields 0 */
        seq_t t1 = (seq_t)n;
        seq_t t2 = (seq_t)(n - 3); /* n-3 > 0 for n>=7 */
        seq_t t3 = (seq_t)(n - 5); /* n-5 > 0 for n>=7 */
        seq_t prod = t1 * t2;
        prod = prod * t3;
        prod = prod / 8;
        return prod;
    }
}
