/*
 * A021371  Decimal expansion of 1/367.
 * https://oeis.org/A021371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n mod 367) * 10 / 367) a(n) is the n-th digit
 *            after the decimal point of 1/367 (a(0) is the first digit after
 *            the decimal point)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 367;          /* modulus for the recurring decimal */
    seq_t result = 1;               /* 10^0 mod MOD */
    seq_t base   = 10 % MOD;        /* 10^1 mod MOD */
    idx_t  exp   = n;               /* exponent to compute 10^n mod MOD */

    while (exp > 0) {
        if (exp & 1) {              /* if current bit of exp is set */
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD; /* square the base */
        exp = exp >> 1;             /* shift exponent right */
    }

    /* digit = floor((10^n mod MOD * 10) / MOD) */
    seq_t digit = (result * 10) / MOD;
    return digit;
}
