/*
 * A021113  Decimal expansion of 1/109.
 * https://oeis.org/A021113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/109) mod 10
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 109 */
    seq_t r   = 1;                /* 10^0 mod 109 */
    seq_t base = 10 % 109;        /* 10 mod 109 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            unsigned long long tmp = (unsigned long long)r * (unsigned long long)base;
            r = (seq_t)(tmp % 109);
        }
        unsigned long long tmp = (unsigned long long)base * (unsigned long long)base;
        base = (seq_t)(tmp % 109);
        exp >>= 1;
    }

    /* Digit at position n is floor( (10^n mod 109) * 10 / 109 ) */
    unsigned long long tmp = (unsigned long long)r * 10ULL;
    seq_t digit = (seq_t)(tmp / 109ULL);
    return digit;
}
