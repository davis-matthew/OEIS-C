/*
 * A021762  Decimal expansion of 1/758.
 * https://oeis.org/A021762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of decimal expansion of 1/758 :
   a(n) = floor(10^n / 758) - 10 * floor(10^(n-1)/758)   */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* persistent cache of digits already produced */
    static unsigned char *digits = NULL;
    static size_t cap = 0, sz = 0;
    /* remainder after the last produced digit (r_m+1) */
    static uint16_t rem = 1;

    const unsigned denom = 758;
    const size_t target = (size_t)n;

    /* initialise on first call */
    if (digits == NULL) {
        cap = 1024;
        digits = (unsigned char *)malloc(cap);
        if (digits == NULL) return 0;   /* allocation failure */
        sz = 0;
        rem = 1;
    }

    /* grow cache to include desired index */
    while (sz <= target) {
        unsigned int tmp = (unsigned int)rem * 10U;
        digits[sz] = (unsigned char)(tmp / denom);
        rem = (uint16_t)(tmp % denom);
        sz++;

        if (sz == cap) {
            size_t newcap = cap << 1;
            unsigned char *newptr = (unsigned char *)realloc(digits, newcap);
            if (newptr == NULL) return 0;   /* allocation failure */
            digits = newptr;
            cap = newcap;
        }
    }

    return (seq_t)digits[target];
}
