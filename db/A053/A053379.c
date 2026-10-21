/*
 * A053379  a(n) contains n digits '7' or '8' and is divisible by 2^n.
 * https://oeis.org/A053379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(8 - [(a(n-1)/2^(n-1)) mod 2]) Initial
 *            value: a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1; 0 is unused */
    seq_t prev = (seq_t)8;                /* a(1) */
    if (n == 1) return prev;

    seq_t power = (seq_t)10;              /* 10^(1) for i = 2  */
    for (idx_t i = 2; i <= n; ++i) {
        /* Determine \[a(i-1)/2^(i-1) mod 2\] as the (i-1)-th bit of a(i-1) */
        seq_t shifted = prev >> (i - 1);
        uint64_t bit   = (uint64_t)(shifted & (seq_t)1);
        /* Addend is 10^(i-1) times (8 - bit) */
        seq_t addend  = power * (seq_t)(8 - bit);
        prev = prev + addend;
        power = power * (seq_t)10;        /* update to 10^(i) for next iteration */
    }
    return prev;
}
