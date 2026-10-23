/*
 * A122630  a(n) = F(n)_F(n) where F() = Fibonacci numbers A000045.
 * https://oeis.org/A122630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer obtained by interpreting the decimal
 *            representation of the nth Fibonacci number F(n) as a number in
 *            base F(n) (i.e. the digits are the decimal digits of F(n).)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 23, u128 n <= 32,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used; offset is 1 */

    /* Fibonacci number F(n) */
    seq_t f = fib(n);

    /* Count decimal digits of f */
    size_t len = 0;
    seq_t t = f;
    const seq_t ten = (seq_t)10;
    while (t != (seq_t)0) {
        t /= ten;
        len++;
    }
    if (len == 0) len = 1;                /* safety, f != 0 for n>=1 */

    /* Collect decimal digits (most–significant first) */
    uint8_t *digits = (uint8_t*)malloc(len * sizeof(uint8_t));
    if (!digits) return (seq_t)0;          /* allocation failure → 0 */

    t = f;
    for (size_t i = 0; i < len; ++i) {
        digits[len - 1 - i] = (uint8_t)(t % ten);
        t /= ten;
    }

    /* Interpret those digits as a number in base f */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        result = result * f + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
