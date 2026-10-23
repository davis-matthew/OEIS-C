/*
 * A115305  a(n) = if n<5 then n else 5*a(floor(n/5)) + 4 - n mod 5.
 * https://oeis.org/A115305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n<5 then n else 5*a(floor(n/5)) + 4 - n mod 5
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)                   /* base values a(1)=1, a(2)=2, a(3)=3, a(4)=4 */
        return (seq_t)n;
    seq_t v = a(n / 5);          /* recursive step on the floor(n/5) term */
    return 5 * v + 4 - (seq_t)(n % 5);
}
