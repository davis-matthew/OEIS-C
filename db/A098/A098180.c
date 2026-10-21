/*
 * A098180  Odd numbers with twice the odd numbers repeated in order between
 *          them.
 * https://oeis.org/A098180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + (n mod 4 < 2 ? 1 : 0)
 * Equivalently: a(n) = n   if n % 4 ∈ {2,3}
 *                a(n) = n+1 if n % 4 ∈ {0,1}
 */
seq_t a(idx_t n)
{
    /* use bitwise & 3 to compute n mod 4 efficiently */
    if ((n & 3) < 2)
        return (seq_t)n + 1;
    return (seq_t)n;
}
