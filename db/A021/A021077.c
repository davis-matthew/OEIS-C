/*
 * A021077  Decimal expansion of 1/73.
 * https://oeis.org/A021077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (10^(n+1) div 73) mod 10 (for n >= 1), a(0) = 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute 10^(n+1) modulo 730 = 73 * 10 using binary exponentiation. */
    const idx_t MOD = 730;          /* 73 * 10 */
    idx_t exp = n;                  /* we will multiply an extra 10 after the loop */
    idx_t res = 1 % MOD;
    idx_t base = 10 % MOD;

    while (exp > 0) {
        if (exp & 1) {
            res = (res * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }
    /* Account for the extra factor of 10 to get 10^(n+1). */
    res = (res * 10) % MOD;

    /* The desired digit is floor(res / 73); here res < 730, so result is 0..9. */
    return (seq_t)(res / 73);
}
