/*
 * A107959  a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(n^2 + 5*n + 5)/720.
 * https://oeis.org/A107959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(n^2 + 5*n + 5)/720
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 580, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p1 = (seq_t)n + 1;                     /* n+1          */
    seq_t p2 = (seq_t)n + 2;                     /* n+2          */
    seq_t p3 = (seq_t)n + 3;                     /* n+3          */
    seq_t p4 = (seq_t)n + 4;                     /* n+4          */
    seq_t p5 = (seq_t)n * (seq_t)n + 5 * (seq_t)n + 5; /* n^2+5n+5 */

    /* Split the squared factors into separate multiplicands */
    seq_t parts[7];
    parts[0] = p1;
    parts[1] = p2;      /* first (n+2)   */
    parts[2] = p2;      /* second (n+2)  */
    parts[3] = p3;      /* first (n+3)   */
    parts[4] = p3;      /* second (n+3)  */
    parts[5] = p4;
    parts[6] = p5;

    /* 720 = 2^4 * 3^2 * 5.  Remove these factors from the parts
       sequentially, so that the remaining product equals a(n). */
    const uint32_t factors[7] = {2,2,2,2,3,3,5};
    for (int i = 0; i < 7; ++i) {
        uint32_t f = factors[i];
        for (int j = 0; j < 7; ++j) {
            if (parts[j] % f == 0) {
                parts[j] /= f;
                break;
            }
        }
    }

    /* Multiply the reduced parts to obtain the final value. */
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result *= parts[i];
    }

    return result;
}
