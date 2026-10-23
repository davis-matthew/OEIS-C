/*
 * A122618  a(n) = n_n, where "N_b" denotes "N read in base b": if N = Sum c_i
 *          10^i then N_b = Sum c_i b^i.
 * https://oeis.org/A122618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n read in base n (interpret decimal digits of n as a
 *            number in base n)
 * Range:     u8 n <= 50, u16 n <= 199, u32 n <= 1623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If the offset is 1, n will never be 0 in valid calls.  Handle it anyway. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    seq_t power  = (seq_t)1;   /* n^0 initially */
    seq_t base   = (seq_t)n;   /* the base */

    idx_t temp = n;
    while (temp > 0)
    {
        uint8_t digit = (uint8_t)(temp % 10ULL); /* 0‑9 digit of decimal n */
        result += (seq_t)digit * power;
        power  *= base;
        temp   /= 10ULL;
    }
    return result;
}
