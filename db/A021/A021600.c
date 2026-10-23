/*
 * A021600  Decimal expansion of 1/596.
 * https://oeis.org/A021600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) div 596) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 5960 and then divide by 596.
       5960 = 596 * 10 ensures the least significant digit
       of the quotient is produced by integer division.  */
    const unsigned long long MOD = 5960ULL;   /* 596 * 10 */
    const unsigned long long DIV = 596ULL;
    idx_t exp = n + 1;                       /* exponent is at most 2^64-1 */

    unsigned long long result = 1ULL;
    unsigned long long base   = 10ULL;

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* result now equals 10^(n+1) mod 5960.  The desired digit is
       the quotient of this number divided by 596.  */
    unsigned long long digit = result / DIV;
    return (seq_t)digit;
}
