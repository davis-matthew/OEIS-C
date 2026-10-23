/*
 * A021477  Decimal expansion of 1/473.
 * https://oeis.org/A021477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/473) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th digit after the decimal point of 1/473.
 * Uses modular exponentiation with modulus 4730 = 473×10.
 * Works for any seq_t that can hold values 0–9 and for any idx_t ≥ 0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD   = 4730ULL;          /* 473 * 10  */
    const uint64_t THRESH = 473ULL;          /* divisor to get the digit */

    uint64_t exponent = (uint64_t)n + 1ULL;  /* 10^(n+1)  */
    uint64_t base     = 10ULL % MOD;        /* base for mod‑exp  */
    uint64_t result   = 1ULL;               /* accumulator  */

    /* fast modular exponentiation: result = 10^(n+1) mod 4730 */
    while (exponent > 0ULL) {
        if (exponent & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exponent >>= 1ULL;
    }

    /* the decimal digit is the quotient of the remainder divided by 473 */
    uint64_t digit = result / THRESH;       /* 0 … 9 */
    return (seq_t)digit;
}
