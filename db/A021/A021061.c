/*
 * A021061  Decimal expansion of 1/57.
 * https://oeis.org/A021061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = floor(10^(n+1)/57) mod 10, i.e. the
 *            (n+1)-th decimal digit of 1/57 is returned
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Long division of 1/57 to generate decimal digits.
       We only need the (n+1)-th digit; remainder stays below 57.   */
    idx_t steps = n + 1;     /* number of decimal digit steps required  */
    uint64_t r = 1;          /* current remainder                              */
    seq_t digit = 0;         /* placeholder for the digit                     */

    for (idx_t i = 0; i < steps; ++i) {
        r *= 10ULL;                  /* multiply remainder by 10                    */
        digit = (seq_t)(r / 57ULL);  /* obtain next decimal digit                   */
        r = r % 57ULL;               /* update remainder                           */
    }
    return digit;
}
