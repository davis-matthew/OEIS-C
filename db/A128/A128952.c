/*
 * A128952  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by the prime 3 and is
 *          not divisible by at least one of the primes 2, 5 and 7.
 * https://oeis.org/A128952
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - floor(10^n/3) - floor(10^n/70) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2   (a(2) is the first term) */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* Undefined for n < 2; return 0 as a safe default */

    seq_t p10 = pow10(n);                /* 10^n */
    seq_t res = p10;                     /* start with 10^n */
    res -= p10 / 3;                      /* subtract floor(10^n/3) */
    res -= p10 / 70;                     /* subtract floor(10^n/70) */
    res += p10 / 210;                    /* add floor(10^n/210) */
    return res;
}
