/*
 * A021283  Decimal expansion of 1/279.
 * https://oeis.org/A021283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(( (10^n) mod 279 ) * 10 / 279)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the nth decimal digit of 1/279 (0‑based) */

seq_t a(idx_t n)
{
    /* denominator of the rational number 1/279 */
    static const seq_t DEN = 279;

    /* fast modular exponentiation: r = 10^n mod DEN */
    seq_t r    = 1;          /* current result (mod DEN)      */
    seq_t base = 10;         /* base (mod DEN)  (10 % DEN)    */
    idx_t e    = n;          /* exponent */

    while (e > 0)
    {
        if (e & 1)
            r = (r * base) % DEN;
        base = (base * base) % DEN;
        e >>= 1;
    }

    /* digit = floor((r * 10) / DEN) */
    seq_t digit = (r * 10) / DEN;
    return digit;
}
