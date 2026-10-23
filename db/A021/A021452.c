/*
 * A021452  Decimal expansion of 1/448.
 * https://oeis.org/A021452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10 * (10^n mod 448) / 448⌋ This is the nth decimal digit
 *            of 1/448 (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 448 using binary exponentiation. */
    const idx_t MOD = 448;
    idx_t exp = n;
    idx_t base = 10 % MOD;   /* equals 10 */
    idx_t result = 1 % MOD;  /* equals 1 */

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* r is the remainder after n digits. Now compute the digit. */
    idx_t digit_int = (10 * result) / MOD;   /* 0 <= digit_int <= 9 */
    return (seq_t)digit_int;
}
