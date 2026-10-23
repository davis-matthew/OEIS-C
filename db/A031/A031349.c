/*
 * A031349  3-multiplicative persistence: number of iterations of "multiply
 *          3rd powers of digits" needed to reach 0 or 1.
 * https://oeis.org/A031349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations applying x → ∏(digit^3) until
 *            x∈{0,1}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to sequence type */
    seq_t cur = (seq_t)n;
    seq_t count = 0;

    /* iterate until the value is 0 or 1 */
    while (cur > (seq_t)1) {
        /* compute the product of the cubes of the digits of cur */
        seq_t prod = (seq_t)1;
        seq_t temp = cur;
        while (temp > (seq_t)0) {
            seq_t digit = temp % (seq_t)10;
            if (digit == (seq_t)0) {
                prod = (seq_t)0;
                break;               /* product is zero, will stop afterward */
            }
            seq_t cube = digit * digit * digit;
            prod = prod * cube;
            temp = temp / (seq_t)10;
        }
        cur = prod;
        count = count + (seq_t)1;
    }
    return count;
}
