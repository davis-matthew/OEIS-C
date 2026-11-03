/*
 * A011628  Legendre symbol (n,233).
 * https://oeis.org/A011628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 1776, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9033 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^116 (mod 233)  with Legendre symbol interpretation:
 *   a(0) = 0
 *   if n % 233 == 0 -> 0
 *   otherwise compute r = n % 233
 *   if r^116 mod 233 == 1 return 1 else return -1
 * a(n) = a(n-233) is automatically satisfied because r = n % 233.
 */
#include <stdint.h>

static inline seq_t legendre_mod233(seq_t v)
{
    if (v == 0)
        return 0;
    /* work in unsigned 32-bit to avoid overflow */
    unsigned int base = (unsigned int)(v % 233u);
    unsigned int result = 1u;
    unsigned int exp = 116u;           /* (233-1)/2 */

    while (exp) {
        if (exp & 1u)
            result = (unsigned int)((result * base) % 233u);
        base = (unsigned int)((base * base) % 233u);
        exp >>= 1u;
    }
    return (result == 1u) ? (seq_t)1 : (seq_t)-1;
}

seq_t a(idx_t n)
{
    /* Reduce index modulo the period 233 */
    idx_t r = n % 233u;
    if (r == 0)
        return 0;
    return legendre_mod233((seq_t)r);
}
