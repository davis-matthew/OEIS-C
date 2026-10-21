/*
 * A070346  a(n) = 3^n mod 37.
 * https://oeis.org/A070346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 37;
    seq_t result = 1;
    seq_t base   = 3 % MOD;   /* 3 */
    idx_t exp = n;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % MOD;
        exp >>= 1U;
        if (exp)
            base = (base * base) % MOD;
    }
    return result;
}
