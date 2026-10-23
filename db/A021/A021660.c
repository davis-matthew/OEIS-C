/*
 * A021660  Decimal expansion of 1/656.
 * https://oeis.org/A021660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=5, and for n>=4, a(n)= {2,4,3,9,0}[
 *            (n-4) mod 5 ]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;

    static const seq_t cycle[5] = {2, 4, 3, 9, 0};
    idx_t idx = (n - 4) % 5;
    return cycle[idx];
}
