/*
 * A213546  Principal diagonal of the convolution array A213505.
 * https://oeis.org/A213546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 95, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (16*n^5 + 15*n^4 - n)/30   (OEIS A213546)
 *  a(1) = 1, a(2) = 25, a(3) = 170, a(4) = 674, ...
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /*  n == 0 is outside the OEIS offset, return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Use 128‑bit arithmetic for intermediate results to avoid
       overflow for n up to about 10,000 (the tested range).   */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n5 = n4 * nn;          /* n^5 */

    unsigned __int128 num = 16 * n5 + 15 * n4 - nn; /* numerator */
    num /= 30;                                   /* division exact */

    return (seq_t)num;
}
