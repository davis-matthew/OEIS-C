/*
 * A021419  Decimal expansion of 1/415.
 * https://oeis.org/A021419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point of 1/415
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the division remainder is always less than 415,
       so a 64‑bit integer is sufficient */
    uint64_t rem = 1ULL;               /* initial remainder 1 */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                  /* shift left one decimal place */
        seq_t dig = (seq_t)(rem / 415ULL);  /* digit = floor(rem/415) */
        rem %= 415ULL;                 /* new remainder for next step */
        if (i == n) return dig;        /* return the n‑th digit */
    }
    return 0;   /* never reached */
}
