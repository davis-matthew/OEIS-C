/*
 * A021307  Decimal expansion of 1/303.
 * https://oeis.org/A021307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=3; a(n)=a(n-1)-a(n-2)+a(n-3) for n>2. This
 *            yields the 4‑cycle 0,0,3,3 repeating
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Using the period 4 property to compute in O(1). */
    switch (n & 3U)
    {
        case 0:
        case 1:
            return (seq_t)0;
        default:          /* cases 2 and 3 */
            return (seq_t)3;
    }
}
