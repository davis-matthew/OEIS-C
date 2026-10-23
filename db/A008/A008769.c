/*
 * A008769  Expansion of (1+x^8)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+1)*(2*n^2 + 4*n + 83 + 9*(-1)^n)/144)
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t part1 = 2 * nn * nn + 4 * nn + 83;
    /* add 9 if n is even, subtract 9 if n is odd */
    if ((n & 1) == 0)
        part1 += 9;
    else
        part1 -= 9;

    seq_t numerator = (nn + 1) * part1;

    /* round = floor((numerator + 72) / 144) for non‑negative numerator */
    return (numerator + 72) / 144;
}
