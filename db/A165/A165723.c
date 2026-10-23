/*
 * A165723  The (d+1)th digit after the decimal point in the decimal
 *          representation of 1/n, where d is the number of digits of n.
 * https://oeis.org/A165723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the (d+1)th digit after the decimal point in 1/n, where d
 *            = number of decimal digits of n
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digit count of n */
    idx_t d = 0;
    idx_t tmp = n;
    while (tmp) {
        d++;
        tmp /= 10;
    }

    /* Position of desired digit (one‑based) */
    idx_t pos = d + 1;

    /* Long division to compute decimal digits of 1/n.
       Use a 128‑bit intermediate to avoid overflow.   */
    uint64_t r = (uint64_t)(1 % n); /* initial remainder */
    seq_t digit = 0;

    for (idx_t i = 0; i < pos; ++i) {
        __uint128_t prod = (__uint128_t)r * 10; /* r * 10 */
        digit = (seq_t)(prod / n);              /* next digit   */
        r = (uint64_t)(prod % n);               /* new remainder */
    }
    return digit;
}
