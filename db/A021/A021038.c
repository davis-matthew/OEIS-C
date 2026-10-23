/*
 * A021038  Decimal expansion of 1/34.
 * https://oeis.org/A021038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/34) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) mod 340 using binary exponentiation. */
    const int MOD = 340;          /* = 10 * 34 */
    int result = 1 % MOD;
    int base   = 10 % MOD;
    idx_t exponent = n + 1;

    while (exponent > 0) {
        if (exponent & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exponent >>= 1;
    }

    /* The digit is floor(result / 34), since result < 340. */
    return (seq_t)(result / 34);
}
