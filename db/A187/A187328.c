/*
 * A187328  a(n) = floor((2-1/sqrt(2))*n); complement of A187338.
 * https://oeis.org/A187328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2-1/sqrt(2))*n)
 * Range:     u8 n <= 11, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* a(0) = 0 */

    /* Promote n only when needed for multiplication. */
    unsigned __int128 sq   = (unsigned __int128)n * n; /* n² (huge-safe) */
    unsigned __int128 hsq  = sq / 2;                   /* floor(n²/2)   */

    /* ⌊n/√2⌋ = ⌊sqrt(floor(n²/2))⌋   */
    seq_t fp = floor_sqrt((seq_t)hsq);                 /* floor(n/√2)  */

    seq_t nn = (seq_t)n;
    /* a(n) = 2n − ⌊n/√2⌋ − 1  (holds for n > 0) */
    return 2 * nn - fp - 1;
}
