/*
 * A021323  Decimal expansion of 1/319.
 * https://oeis.org/A021323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n in the decimal expansion of 1/319
 *            (OEIS A021323)
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;      /* current remainder after dividing by 319 */
    seq_t digit = 0;  /* digit to be returned */

    /* iterate n+1 times to obtain the (n+1)th digit after the decimal point */
    for (idx_t i = 0; i <= n; ++i) {
        unsigned long long tmp = (unsigned long long)r * 10ULL; /* r < 319, safe product */
        digit = (seq_t)(tmp / 319ULL);                         /* next decimal digit */
        r = (seq_t)(tmp % 319ULL);                             /* new remainder */
    }
    return digit;
}
