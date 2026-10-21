/*
 * A214085  a(n) = n^2 * (n^4 - n^2 + n + 1) / 2.
 * https://oeis.org/A214085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n^4 - n^2 + n + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit integers for intermediate products */
    unsigned __int128 N = (uint64_t)n;          /* promote index to 128 bits */
    unsigned __int128 n2 = N * N;              /* n^2 */
    unsigned __int128 n4 = n2 * n2;            /* n^4 */
    unsigned __int128 term = n4 - n2 + N + 1;   /* n^4 - n^2 + n + 1 */
    unsigned __int128 num  = n2 * term;        /* n^2 * (… ) */
    num /= 2;                                 /* divide by 2 */

    return (seq_t)num;                         /* cast back to seq_t */
}
