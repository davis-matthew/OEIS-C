/*
 * A021152  Decimal expansion of 1/148.
 * https://oeis.org/A021152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 for n=0,1; a(n) = {6,7,5}[(n-2) mod 3] for n>=2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[3] = {6, 7, 5};
    if (n < 2)
        return 0;
    return period[(n - 2) % 3];
}
