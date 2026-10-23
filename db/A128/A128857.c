/*
 * A128857  a(n) = least number m beginning with 1 such that the quotient m/n
 *          is obtained merely by shifting the leftmost digit 1 of m to the
 *          right end.
 * https://oeis.org/A128857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed as decimal period of n/(10n-1) */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;                       /* cast idx_t to seq_t */
    idx_t denom = (idx_t)(nn * (seq_t)10u - (seq_t)1u); /* 10n – 1  */
    idx_t remainder = n;                          /* initial remainder   */
    seq_t result = (seq_t)0u;                     /* accumulates a(n)     */

    do {
        idx_t tmp = remainder * (idx_t)10u;       /* remainder × 10       */
        seq_t digit = (seq_t)(tmp / denom);       /* next decimal digit   */
        remainder = tmp % denom;                  /* new remainder       */
        result = result * (seq_t)10u + digit;     /* append digit        */
    } while (remainder != n);                     /* stop when cycle repeats */

    return result;
}
