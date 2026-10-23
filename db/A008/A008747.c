/*
 * A008747  Expansion of (1+x^4)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+1)^2/6) = ((n+1)^2 + 5)/6
 * Range:     u8 n <= 14, u16 n <= 625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute (n+1)^2 */
    seq_t np1 = (seq_t)(n + 1);
    seq_t sq   = np1 * np1;

    /* Compute ceil division: ((n+1)^2 + 5)/6 */
    return (sq + 5) / 6;
}
