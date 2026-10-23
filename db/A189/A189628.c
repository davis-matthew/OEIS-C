/*
 * A189628  Fixed point of the morphism 0->001, 1->010.
 * https://oeis.org/A189628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k-2)=0, a(3k-1)=a(k), a(3k)=1-a(k) (k ≥ 1)
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) for n ≥ 1, recursion per the recurrence relations */
    if ((n % 3) == 1)          /* 3k-2 case */
        return (seq_t)0;
    if ((n % 3) == 2)          /* 3k-1 case */
        return a((n + 1) / 3);
    /* (n % 3) == 0 -> 3k case */
    return (seq_t)1 - a(n / 3);
}
