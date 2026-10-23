/*
 * A185452  Image of n under the map n -> n/2 if n even, (5*n+1)/2 if n odd.
 * https://oeis.org/A185452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+1-(4*n+1)*(-1)^n)/4
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mapping rule: a(n) = n/2   if n is even, (5*n+1)/2 if n is odd */
seq_t a(idx_t n)
{
    if ((n & 1) == 0)
    {
        return (seq_t)(n / 2);
    }
    else
    {
        return ((seq_t)5 * n + 1) / 2;
    }
}
