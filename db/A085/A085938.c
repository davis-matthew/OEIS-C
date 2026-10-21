/*
 * A085938  a(n) is the (n+1)-digit number in which the first digit is 1 and
 *          the subsequent digits increase by steps of n (mod 10).
 * https://oeis.org/A085938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the (n+1) digits d0…dn where * d0 = 1
 *            and dk+1 = (dk + n) mod 10 for k = 0…n-1. * The resulting digit
 *            sequence is interpreted as a base‑10 integer
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t, if needed */

seq_t a(idx_t n) {
    seq_t res = 1;            /* first digit */
    seq_t cur = 1;            /* current digit */
    seq_t step = (seq_t)n;    /* step size */
    seq_t mod10 = (seq_t)10;  /* modulus value */

    /* Generate the remaining n digits */
    for (idx_t i = 1; i <= n; ++i) {
        cur = (cur + step) % mod10;
        res = res * (seq_t)10 + cur;
    }

    return res;
}
