/*
 * A191760  Digital root of the n-th odd square.
 * https://oeis.org/A191760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of the square of the n-th odd number i.e.
 *            a(n) = dr((2n-1)^2) = 1 + (( (2n-1)^2 – 1 ) mod 9), with dr(m) =
 *            9 if m mod 9 == 0, otherwise dr(m) = m mod 9
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the value type early to avoid intermediate promotions. */
    seq_t tn = (seq_t)n;

    /* (2n - 1) as a seq_t.  (tn << 1) == 2*n. */
    seq_t t   = (tn << 1) - 1;          /* the n-th odd number */
    seq_t tmod = t % 9;                 /* (2n-1) mod 9 */
    seq_t r    = (tmod * tmod) % 9;     /* (2n-1)^2 mod 9 */

    /* Digital root: 9 if remainder is 0, else the remainder itself. */
    return (r == 0) ? 9 : r;
}
