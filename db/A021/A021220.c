/*
 * A021220  Decimal expansion of 1/216.
 * https://oeis.org/A021220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) equals the n-th digit after the decimal point in the
 *            expansion of 1/216. It can be computed by modular
 *            exponentiation: a(n) = ⌊(10^(n) * 10) / 216⌋, where 10^n is
 *            taken modulo 216
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n;               /* working exponent */
    seq_t mod = 216;           /* denominator of the fraction 1/216 */
    seq_t r   = 1;             /* r = 10^k mod mod */
    seq_t base = 10;           /* base = 10^1 mod mod */

    /* binary exponentiation to compute 10^k mod 216 */
    while (k) {
        if (k & 1) r = (r * base) % mod;
        base = (base * base) % mod;
        k >>= 1;
    }

    /* digit extraction: (r * 10) / 216 gives the desired digit 0‑9 */
    seq_t d = (r * 10) / mod;
    return d;
}
