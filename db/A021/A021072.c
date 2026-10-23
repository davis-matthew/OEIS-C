/*
 * A021072  Decimal expansion of 1/68.
 * https://oeis.org/A021072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 68) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th digit after the decimal point of 1/68
   where a(0) is the first decimal digit. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus for 10^(n+1) modulo (68*10)=680 */
    const uint64_t MOD = 680ULL;
    const uint64_t BASE = 10ULL;
    uint64_t exp = n + 1;          /* exponent n+1 */
    uint64_t res = 1ULL;           /* result of powmod 10^(n+1) mod 680 */
    uint64_t base = BASE % MOD;

    /* Binary exponentiation */
    while (exp > 0) {
        if (exp & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* Digit = floor(res / 68) */
    return (seq_t)(res / 68ULL);
}
