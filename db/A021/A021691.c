/*
 * A021691  Decimal expansion of 1/687.
 * https://oeis.org/A021691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th decimal digit of the expansion of 1/687
   (a(0)=0, a(1)=0, a(2)=1, a(3)=4, …) */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint32_t rem = 1;  /* current remainder, < 687  */
    uint32_t digit = 0;

    for (idx_t i = 0; i <= n; i++) {
        rem *= 10;                 /* multiply remainder by 10 */
        digit = rem / 687;          /* next decimal digit   */
        rem   = rem % 687;          /* update remainder     */
    }
    return (seq_t)digit;
}
