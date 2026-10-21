/*
 * A200860  Multiples of 682.
 * https://oeis.org/A200860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 682*n (recurrence: a(n)=2*a(n-1)-a(n-2), a(0)=0,
 *            a(1)=682)
 * Range:     u8 n <= 0, u16 n <= 96, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)682;
}
