/*
 * A021850  Decimal expansion of 1/846.
 * https://oeis.org/A021850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point of the fraction 1/846
 *            (0‑indexed). The value is computed by ordinary long division,
 *            keeping the remainder modulo 846
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned long long r = 1;      /* current remainder */
    unsigned long long digit = 0;  /* digit that will be returned */

    /* Perform the division n+1 times to obtain the digit at position n */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10ULL;          /* bring down next decimal digit */
        digit = r / 846ULL;     /* current digit (0‑9) */
        r = r % 846ULL;         /* new remainder */
    }

    return (seq_t)digit;
}
