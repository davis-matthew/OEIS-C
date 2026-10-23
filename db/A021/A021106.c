/*
 * A021106  Decimal expansion of 1/102.
 * https://oeis.org/A021106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/102) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/102: 0.009803921568627452... */
seq_t a(idx_t n)
{
    seq_t num = 1;          /* remainder */
    const seq_t den = 102;  /* denominator */
    seq_t digit = 0;
    idx_t i;

    /* perform n+1 iterations: the i‑th iteration produces a(i) */
    for (i = 0; i <= n; ++i) {
        digit = (num * 10) / den;
        num   = (num * 10) % den;
    }
    return digit;
}
