/*
 * A207256  Number of 5 X n 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^5 + 160*n^4 + 220*n^3 - 160*n^2 + 16*n) / 15
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 102, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (valid for all n ≥ 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a wider integer type for intermediate arithmetic.
       __int128 is available on GCC/Clang and is sufficient for n up to 10⁶
       (which covers all test cases).  The division by 15 yields an integer. */
    __int128 N   = (__int128) n;
    __int128 n2  = N * N;          /* n^2   */
    __int128 n3  = n2 * N;         /* n^3   */
    __int128 n4  = n3 * N;         /* n^4   */
    __int128 n5  = n4 * N;         /* n^5   */

    __int128 num = 4 * n5            /* 4*n^5   */
                 + 160 * n4          /* 160*n^4 */
                 + 220 * n3          /* 220*n^3 */
                 - 160 * n2          /* -160*n^2*/
                 + 16 * N;           /* 16*n    */

    return (seq_t)(num / 15);
}
