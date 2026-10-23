/*
 * A008611  a(n) = a(n-3) + 1, with a(0)=a(2)=1, a(1)=0.
 * https://oeis.org/A008611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-3) + 1 with a(0)=1, a(1)=0, a(2)=1. Closed form: a(n)
 *            = n/3 + 1 if n%3 != 1, otherwise a(n) = n/3
 * Range:     u8 n <= 764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    int r  = n % 3;
    if (r == 1)
        return (seq_t)k;
    /* r == 0 or 2 */
    return (seq_t)(k + 1);
}
