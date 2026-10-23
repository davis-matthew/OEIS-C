/*
 * A021536  Decimal expansion of 1/532.
 * https://oeis.org/A021536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊ 10 · (10^n mod 532) / 532 ⌋
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 8, …  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 532   */
    const unsigned long long MOD = 532ULL;          /* 532 fits in 64‑bit */
    unsigned long long r  = 1 % MOD;                /* r = 10^0 mod 532 */
    unsigned long long base = 10 % MOD;             /* base = 10^1 mod 532 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1ULL)
            r = (r * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* The nth digit after the decimal point of 1/532  */
    unsigned long long digit = (10ULL * r) / MOD;
    return (seq_t)digit;
}
