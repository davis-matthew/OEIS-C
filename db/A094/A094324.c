/*
 * A094324  a(n) = 5 written in base n.
 * https://oeis.org/A094324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 written in base n, interpreted as a decimal integer
 * Range:     u8 none, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 the unary representation is 11111; for n ≥ 2 the usual base‑n digits are used. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 11111;                 /* base‑1 (unary) representation of 5 */

    /* For bases 2 and above, compute the decimal integer that encodes
       the digits of 5 in that base.  Because 5 < base^3 for every base ≥ 2,
       at most three decimal digits will be produced. */
    seq_t value   = 5;      /* the number to convert */
    seq_t result  = 0;      /* accumulated decimal result */
    seq_t mult    = 1;      /* place value multiplier (10^k) */
    idx_t  base   = n;

    while (value > 0)
    {
        seq_t digit = value % base;          /* current base‑n digit (0..base-1) */
        result += digit * mult;              /* append digit in decimal */
        mult   *= 10;                        /* next decimal place */
        value  /= base;                      /* move to next digit */
    }

    return result;     /* decimal integer representing 5 in base n */
}
