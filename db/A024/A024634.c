/*
 * A024634  n written in fractional base 5/4.
 * https://oeis.org/A024634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n written in fractional base 5/4
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 74, u64 n <= 739,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (digits 0–4, carry 4 when a digit would exceed 4) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t cap = 64;                 /* capacity of digit buffer      */
    uint8_t *d = (uint8_t *)malloc(cap);
    if (!d) return 0;                /* fallback on allocation failure */

    size_t len = 1;                  /* current number of digits       */
    d[0] = 0;                        /* starting value 0               */

    /* increment digits n times to reach the n‑th representation */
    for (idx_t i = 1; i <= n; ++i) {
        size_t j = 0;
        unsigned char carry = 1;      /* add one to the least significant digit */

        while (carry && j < len) {
            unsigned int s = d[j] + carry;
            if (s >= 5) {              /* digit would exceed 4            */
                s -= 5;                /* subtract a (5)                  */
                carry = 4;             /* carry value is b (4)            */
            } else
                carry = 0;
            d[j] = (uint8_t)s;
            ++j;
        }

        if (carry) {                  /* handle final carry              */
            if (len == cap) {
                cap <<= 1;              /* double buffer                   */
                d = (uint8_t *)realloc(d, cap);
            }
            d[len++] = carry;
        }
    }

    /* convert digit array (most significant first) to a decimal integer */
    seq_t val = 0;
    for (size_t i = len; i > 0; --i) {
        val = val * 10 + d[i - 1];
    }

    free(d);
    return val;
}
