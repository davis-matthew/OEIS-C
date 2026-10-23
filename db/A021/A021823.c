/*
 * A021823  Decimal expansion of 1/819.
 * https://oeis.org/A021823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-6), a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=2, a(5)=1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The period of the decimal expansion of 1/819 is 6:
       digits: 0,0,1,2,2,1   (repeating). */
    static const seq_t pattern[6] = {0, 0, 1, 2, 2, 1};

    /* n is an unsigned 64‑bit type; n % 6 yields an idx_t in 0..5. */
    return pattern[n % 6];
}
