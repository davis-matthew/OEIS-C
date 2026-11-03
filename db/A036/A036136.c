/*
 * A036136  a(n) = 3^n mod 89.
 * https://oeis.org/A036136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 89
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9932 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_mod_89(seq_t a, seq_t b)
{
    seq_t res = 0;
    for (idx_t i = 0; i < (idx_t)b; ++i)
        res = (res + a) % 89;
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (n > 0) {
        if (n & 1)
            result = mul_mod_89(result, base);
        base = mul_mod_89(base, base);
        n >>= 1;
    }
    return result;
}
