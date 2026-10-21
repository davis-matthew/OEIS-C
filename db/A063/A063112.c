/*
 * A063112  a(1) = 1; a(n+1) = a(n) + product of nonzero digits of a(n) when
 *          written in base 3. Display sequence in base 3.
 * https://oeis.org/A063112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n+1)=a(n)+product of nonzero base‑3 digits of a(n)
 * Range:     u8 n <= 14, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1u) return 0;              /* value undefined for n<1 */

    seq_t val = 1;                     /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t prod = 1;
        seq_t t    = val;
        while (t > 0) {
            seq_t d = t % 3;
            if (d != 0) prod *= d;
            t /= 3;
        }
        val += prod;
    }

    /* Convert the decimal value of a(n) into a base‑3 integer representation */
    seq_t rep = 0;
    seq_t tmp = val;

    /* buffer for base‑3 digits (max digits < 200 for all tested n) */
    uint8_t digits[200];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    for (int k = len - 1; k >= 0; --k) {
        rep = rep * 10 + digits[k];
    }

    return rep;
}
