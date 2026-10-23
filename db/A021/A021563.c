/*
 * A021563  Decimal expansion of 1/559.
 * https://oeis.org/A021563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/559) mod 10 (digit of the decimal
 *            expansion of 1/559 after the decimal point)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 559 */
    seq_t r = (seq_t)1;
    const seq_t MOD = (seq_t)559;
    for (idx_t i = 0; i < n; ++i) {
        r = (r * (seq_t)10) % MOD;
    }
    /* The digit is floor(10 * r / 559) */
    seq_t digit = (r * (seq_t)10) / MOD;
    return digit;
}
