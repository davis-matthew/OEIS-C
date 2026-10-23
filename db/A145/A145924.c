/*
 * A145924  Last digit of A145923(n).
 * https://oeis.org/A145924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = a(n-5) with a(0)=9, a(1)=3, a(2)=5, a(3)=5, a(4)=3 –
 *            period 5
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5u;
    if (r == 0) return 9;
    if (r == 1) return 3;
    if (r == 2) return 5;
    if (r == 3) return 5;   /* r == 4 falls through */
    return 3;
}
