/*
 * A021610  Decimal expansion of 1/606.
 * https://oeis.org/A021610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/606:
   a(0) = a(1) = 0
   a(n) = {1,6,5,0}[ (n-2) mod 4 ] for n ≥ 2
*/
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    switch ((n - 2) & 3) /* use bitmask for mod 4 */
    {
        case 0: return 1;
        case 1: return 6;
        case 2: return 5;
        /* case 3: */ return 0;
    }
    return 0; /* unreachable, but keeps compiler happy */
}
