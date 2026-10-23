/*
 * A023381  a(n+1) = a(n) converted to base 8 from base 5 (written in base
 *          10).
 * https://oeis.org/A023381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) interpreted as a base‑8 number whose digits are
 *            the base‑5 digits of a(n). a(1) = 5. This implements a(n) by
 *            iterating the conversion n−1 times
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (n == 1) return (seq_t)5;         /* first term */

    seq_t cur = (seq_t)5;                /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        seq_t temp = cur;                /* work on a(i) */
        seq_t next = (seq_t)0;           /* a(i+1) will be built here */
        seq_t mult = (seq_t)1;           /* 8^position */

        while (temp > (seq_t)0) {
            seq_t digit = temp % (seq_t)5;     /* base‑5 digit */
            next += digit * mult;              /* place in base‑8 value */
            mult *= (seq_t)8;
            temp /= (seq_t)5;
        }
        cur = next;                                   /* advance */
    }
    return cur;                      /* a(n) */
}
