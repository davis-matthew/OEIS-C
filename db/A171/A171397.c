/*
 * A171397  Write n in base 10, but then read it as if it were written in base
 *          11: if n = Sum_{i >= 0} d_i*10^i, with 0 <= d_i <= 9, then a(n) =
 *          Sum_{i >= 0} d_i*11^i.
 * https://oeis.org/A171397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i≥0} ((n / 10^i) mod 10) * 11^i
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. interpret the decimal digits of n as a base‑11 number */
seq_t a(idx_t n)
{
    seq_t res   = 0;
    seq_t power = 1;   /* 11^i */
    idx_t t = n;

    while (t != 0) {
        seq_t digit = (seq_t)(t % 10);   /* least significant decimal digit */
        res += digit * power;            /* add digit * 11^i */
        power *= 11;                     /* move to next position */
        t /= 10;                          /* drop processed digit */
    }

    return res;
}
