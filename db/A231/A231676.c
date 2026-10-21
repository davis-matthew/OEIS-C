/*
 * A231676  a(n) = Sum_{i=0..n} digsum_7(i), where digsum_7(i) = A053828(i).
 * https://oeis.org/A231676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digit_sum_base7(i)
 * Range:     u8 n <= 44, u16 n <= 5317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here digit_sum_base7(i) is the sum of the digits of i in base 7 (A053828). */
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t base = (seq_t)7;           /* base for digit sum */
    const seq_t sum_digits_0_to_6 = (seq_t)21; /* 0+1+...+6 */
    seq_t sum = 0;                        /* result to return */
    seq_t nn = (seq_t)n + 1;              /* count of numbers from 0 to n inclusive */
    seq_t pow = 1;                        /* 7^p for current digit position */

    /* Iterate over all digit positions that exist in numbers up to n */
    while (pow <= (seq_t)n)
    {
        seq_t cycle = pow * base;                 /* length of one full cycle */
        seq_t full_cycles = nn / cycle;           /* how many full cycles */
        seq_t rem = nn % cycle;                   /* remaining numbers */

        /* Sum from all full cycles */
        sum += full_cycles * pow * sum_digits_0_to_6;

        /* Process the remaining part */
        seq_t high = rem / pow;                   /* number of complete digit values */
        seq_t low  = rem % pow;                   /* remaining numbers for the next digit value */

        /* Sum of digits for incomplete cycle:
           pow * (0+1+...+(high-1)) + low * high
           where (0+1+...+(high-1)) = high*(high-1)/2   */
        sum += pow * (high * (high - 1) / 2) + low * high;

        pow *= base;                              /* move to next digit position */
    }

    return sum;
}
