/*
 * A163277  a(n) = n^7*(n+1)^2/2.
 * https://oeis.org/A163277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^7*(n+1)^2/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;        /* convert index to seq_t */
    /* compute n^7 */
    seq_t n2 = n_seq * n_seq;      /* n^2 */
    seq_t n3 = n2 * n_seq;         /* n^3 */
    seq_t n4 = n3 * n_seq;         /* n^4 */
    seq_t n5 = n4 * n_seq;         /* n^5 */
    seq_t n6 = n5 * n_seq;         /* n^6 */
    seq_t n7 = n6 * n_seq;         /* n^7 */
    /* compute (n+1)^2 */
    seq_t np1 = n_seq + 1;         /* n+1 */
    seq_t np1_2 = np1 * np1;       /* (n+1)^2 */

    /* avoid overflow by dividing the even factor first */
    if ((np1_2 & 1) == 0)          /* (n+1)^2 is even */
        np1_2 >>= 1;               /* np1_2 /= 2 */
    else
        n7 >>= 1;                  /* n7 /= 2 (must be even when n is odd) */

    /* final result */
    seq_t result = n7 * np1_2;     /* n^7*(n+1)^2/2 */
    return result;
}
