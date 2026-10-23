/*
 * A021328  Decimal expansion of 1/324.
 * https://oeis.org/A021328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 324) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^n (mod 324) using fast exponentiation */
    idx_t mod   = 324;           /* constant modulus */
    idx_t base  = 10 % mod;      /* base for exponentiation */
    idx_t exp   = n;             /* exponent */
    idx_t rem   = 1;             /* running remainder, 1 = 10^0 mod 324 */

    while (exp) {
        if (exp & 1U)
            rem = (rem * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }

    /* The nth decimal digit is floor(10 * rem / 324) */
    seq_t digit = (seq_t)((rem * 10ULL) / mod);
    return digit;
}
