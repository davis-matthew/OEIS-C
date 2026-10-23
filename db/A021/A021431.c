/*
 * A021431  Decimal expansion of 1/427.
 * https://oeis.org/A021431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) mod 4270) / 427)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the (n+1)-th decimal digit of 1/427. */
seq_t a(idx_t n)
{
    const idx_t MOD = 4270;
    idx_t exp = n + 1;
    idx_t result = 1 % MOD;
    idx_t base   = 10 % MOD;

    while (exp)
    {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return (seq_t)(result / 427);
}
