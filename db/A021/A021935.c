/*
 * A021935  Decimal expansion of 1/931.
 * https://oeis.org/A021935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * (10^n mod 931) / 931)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int MOD = 931;
    int result = 1 % MOD;          /* 10^0 mod 931 */
    int base   = 10 % MOD;         /* 10^1 mod 931 */
    idx_t exp  = n;

    while (exp) {
        if (exp & 1u)
            result = (result * base) % MOD;
        exp >>= 1u;
        if (exp)        /* square only if more bits remain */
            base = (base * base) % MOD;
    }

    int digit = (result * 10) / MOD;   /* 0 … 9 */
    return (seq_t)digit;
}
