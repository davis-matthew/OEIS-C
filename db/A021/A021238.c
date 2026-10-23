/*
 * A021238  Decimal expansion of 1/234.
 * https://oeis.org/A021238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(0)=0, a(1)=0, a(n)=cycle[(n-2) mod 6] for n≥2 where
 *            cycle={4,2,7,3,5,0}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char cycle[6] = {4, 2, 7, 3, 5, 0};
    if (n < 2)
        return 0;
    return (seq_t)cycle[(n - 2) % 6];
}
