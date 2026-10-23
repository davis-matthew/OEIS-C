/*
 * A021677  Decimal expansion of 1/673.
 * https://oeis.org/A021677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod 673)) / 673) The nth digit after
 *            the decimal point in 1/673
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t */

/* multiply two seq_t values modulo MOD, using a wider
   intermediate type to avoid overflow on small seq_t widths */
static seq_t mul_mod(seq_t a, seq_t b, seq_t MOD)
{
    return (seq_t)(((uint64_t)a * b) % MOD);
}

/* compute 10^exp mod 673, where exp is an idx_t */
static seq_t pow10_mod(idx_t exp)
{
    const seq_t MOD = 673;
    seq_t result = 1 % MOD;
    seq_t base   = 10 % MOD;

    while (exp > 0)
    {
        if (exp & 1)
            result = mul_mod(result, base, MOD);
        base = mul_mod(base, base, MOD);
        exp >>= 1;
    }
    return result;
}

/* external function required by the exercise */
seq_t a(idx_t n)
{
    /* remainder before the nth digit is 10^n mod 673 */
    seq_t rem = pow10_mod(n);

    /* digit = floor((rem*10)/673).  Use a wider type for the
       intermediate multiplication to preserve correctness when
       seq_t is 8‑bit or 16‑bit. */
    seq_t digit = (seq_t)(((uint64_t)rem * 10u) / 673u);
    return digit;
}
