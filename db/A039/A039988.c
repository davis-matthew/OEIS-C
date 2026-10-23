/*
 * A039988  k th digit of a(n) is the number of different digits within 1 of k
 *          (not including k).
 * https://oeis.org/A039988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A039988:  the decimal number whose k‑th digit equals the
 * number of distinct digits among its adjacent neighbors.
 * If n % 4 == 3 the pattern is `1` followed by (2,2,1) repeated k times,
 * where k = (n+1)/4.  Otherwise the number is all 1’s.  The length equals
 * n + 2 - floor((n+1)/4). */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Length of the decimal representation */
    idx_t m   = (n + 1) >> 2;               /* floor((n+1)/4)   */
    idx_t len = n + 2 - m;                  /* total digits      */

    seq_t val = 0;

    if ((n & 3) == 3) {                    /* pattern with 2's */
        idx_t k = m;                       /* k = (n+1)/4      */
        val = 1;                           /* leading '1'      */
        for (idx_t i = 0; i < k; ++i) {
            val = val * 10 + 2;            /* next digit 2     */
            val = val * 10 + 2;            /* next digit 2     */
            val = val * 10 + 1;            /* next digit 1     */
        }
    } else {                               /* all 1's */
        for (idx_t i = 0; i < len; ++i) {
            val = val * 10 + 1;
        }
    }
    return val;
}
