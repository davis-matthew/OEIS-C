/*
 * A021292  Decimal expansion of 1/288.
 * https://oeis.org/A021292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1)) / 288) % 10, i.e., the nth decimal digit
 *            of 1/288 after the decimal point
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^n (mod 288).  This fits in a few bits, so
       we can use a simple fast‑exponentiation routine. */
    const seq_t MOD = 288;
    seq_t result = 1;
    seq_t base   = 10 % MOD;          /* base = 10 */
    idx_t exp    = n;

    /* Binary exponentiation */
    while (exp) {
        if (exp & 1)
            result = (seq_t)(((uint64_t)result * base) % MOD);
        base = (seq_t)(((uint64_t)base * base) % MOD);
        exp >>= 1;
    }

    /* Compute the digit: floor((10^(n+1))/288) % 10.
       Since result = 10^n (mod 288), the next quotient is
       floor((result * 10) / 288). */
    seq_t digit = (seq_t)(((uint64_t)result * 10) / MOD);
    return digit % 10;
}
