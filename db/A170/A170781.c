/*
 * A170781  a(n) = n^8*(n^9 + 1)/2.
 * https://oeis.org/A170781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^9 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 14, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t.  
     * In C, the cast is explicit; in C++ the constructor will handle it. */
    seq_t N = (seq_t)n;

    /* Compute n^2, n^4, n^8 and n^9 using only seq_t operations. */
    seq_t n2 = N * N;          /* n^2                 */
    seq_t n4 = n2 * n2;        /* n^4                 */
    seq_t n8 = n4 * n4;        /* n^8                 */
    seq_t n9 = n8 * N;         /* n^9                 */

    /* One of (n^8) or (n^9 + 1) is even, so divide the even factor
     * by two first to keep intermediate values in range.  For 64‑bit
     * builds this avoids overflow on the final multiplication. */
    seq_t t1, t2;
    if ((N & (seq_t)1) == (seq_t)0) {          /* n even  */
        t1 = n8 / (seq_t)2;                    /* (n^8)/2 */
        t2 = n9 + (seq_t)1;                    /* n^9 + 1 */
    } else {                                   /* n odd   */
        t1 = n8;                               /* n^8     */
        t2 = (n9 + (seq_t)1) / (seq_t)2;       /* (n^9+1)/2 */
    }

    return t1 * t2;
}
