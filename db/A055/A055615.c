/*
 * A055615  a(n) = n * mu(n), where mu is the Möbius function A008683.
 * https://oeis.org/A055615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) = n * mu(n) (mu is the Möbius function)
 * Range:     i8 n <= 128, i16 n <= 9218, i32 n <= 9218, i64 n <= 9218,
 *            i128 n <= 9218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple trial‑division implementation of the Möbius function. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The Möbius function is defined only for positive integers. */
    if (n == 0) return 0;          /* safe guard; not part of sequence */

    seq_t sign = 1;                /* current sign of mu(n)   */
    idx_t tmp  = n;                /* value to factorise      */

    /* Handle factor 2 separately to keep the loop over odd numbers. */
    if ((tmp & 1) == 0) {
        unsigned int cnt = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;            /* divide by 2 */
            ++cnt;
            if (cnt > 1) return 0;    /* squared prime factor => mu(n) = 0 */
        }
        sign = -sign;              /* toggle sign for one factor of 2 */
    }

    /* Test odd divisors up to sqrt(tmp).  Use division to avoid overflow. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
                if (cnt > 1) return 0;    /* squared factor */
            }
            sign = -sign;          /* toggle sign for this prime */
        }
    }

    /* If tmp > 1, it is a prime factor larger than sqrt(original n). */
    if (tmp > 1) {
        sign = -sign;
    }

    return (seq_t)n * sign;         /* a(n) = n * mu(n) */
}
