/*
 * A171649  a(n) = 0+1+2+...+n in lunar arithmetic in base 8 written in base
 *          10.
 * https://oeis.org/A171649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i=0}^{n} i in lunar arithmetic (base 8)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n; /* current quotient floor(n / 8^k) */
    seq_t res = 0;        /* result in base‑10 */
    seq_t fac = 1;        /* 8^k factor */

    while (m > 0) {
        seq_t d = (m > 7 ? 7 : m);   /* maximum digit for this position */
        res += d * fac;
        fac *= 8;
        m /= 8;
    }

    return res;
}
