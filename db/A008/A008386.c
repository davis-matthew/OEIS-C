/*
 * A008386  Crystal ball sequence for A_5 lattice.
 * https://oeis.org/A008386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(63*n^4+126*n^3+217*n^2+154*n+60)/60
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t tn = (seq_t)n;

    /* Compute the polynomial part: 63*n^4 + 126*n^3 + 217*n^2 + 154*n + 60 */
    seq_t n2 = tn * tn;                     /* n^2 */
    seq_t n3 = n2 * tn;                     /* n^3 */
    seq_t n4 = n3 * tn;                     /* n^4 */

    seq_t p = (seq_t)63 * n4
            + (seq_t)126 * n3
            + (seq_t)217 * n2
            + (seq_t)154 * tn
            + (seq_t)60;

    /* Multiply by (2*n + 1) and divide by 60 using 128‑bit arithmetic
       to avoid overflow of the intermediate product. */
    seq_t t = (seq_t)2 * tn + (seq_t)1;

    unsigned __int128 prod = (unsigned __int128)t * (unsigned __int128)p;
    prod /= 60;

    return (seq_t)prod;
}
