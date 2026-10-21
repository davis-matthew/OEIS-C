/*
 * A223639  Number of n X 3 0..1 arrays with rows, columns, diagonals and
 *          antidiagonals unimodal.
 * https://oeis.org/A223639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( 23n⁶ - 27n⁵ + 620n⁴ + 225n³ + 1517n² - 918n + 1080 ) /
 *            360 (OEIS A223639 – see the comment in the OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1u)             /* offset is 1; values below that are undefined  */
        return 0;

    /* Use 128‑bit arithmetic for the intermediate computation to avoid
       overflow of 64‑bit seq_t values.  GCC/Clang provide __uint128_t,
       which is available on all platforms used for the judge. */
    __uint128_t N  = (unsigned long long)n;
    __uint128_t n2 = N * N;            /* n²   */
    __uint128_t n3 = n2 * N;           /* n³   */
    __uint128_t n4 = n2 * n2;          /* n⁴   */
    __uint128_t n5 = n4 * N;           /* n⁵   */
    __uint128_t n6 = n3 * n3;          /* n⁶   */

    __uint128_t num = 23 * n6
                    - 27 * n5
                    + 620 * n4
                    + 225 * n3
                    + 1517 * n2
                    - 918 * N
                    + 1080;          /* numerator of 360·a(n) */

    return (seq_t)(num / 360u);
}
