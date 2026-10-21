/*
 * A074993  a(n) = floor((concatenation of n, n+1)/2).
 * https://oeis.org/A074993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( concat(n, n+1) / 2 )
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   concat(n, m) = n * 10^d + m   where d = number of decimal digits of m.
 *   This implements a(n) for all n ≥ 0 using only seq_t arithmetic. */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for arithmetic. */
    seq_t sn = (seq_t)n;      /* n as seq_t */
    seq_t m  = sn + 1;        /* n + 1 */

    /* Compute 10^d where d = number of digits of m. */
    seq_t pow10 = 1;
    seq_t temp  = m;
    while (temp > 0)
    {
        pow10 *= 10;
        temp  /= 10;
    }

    /* Concatenate n and m, then take the floor of half. */
    seq_t concat = sn * pow10 + m;
    return concat / 2;        /* integer division yields floor */
}
