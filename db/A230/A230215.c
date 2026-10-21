/*
 * A230215  Numbers divisible by 3 or 13.
 * https://oeis.org/A230215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+15) = a(n) + 39, with a(1)=3, a(2)=6, a(3)=9, a(4)=12,
 *            a(5)=13, a(6)=15, a(7)=18, a(8)=21, a(9)=24, a(10)=26, a(11)=27,
 *            a(12)=30, a(13)=33, a(14)=36, a(15)=39
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases for the first 15 terms */
    static const seq_t base[15] = {
        3, 6, 9, 12, 13,
        15, 18, 21, 24, 26,
        27, 30, 33, 36, 39
    };

    /* distance from the beginning of the current cycle of length 15 */
    idx_t r = ((n - 1) % 15) + 1;          /* 1 .. 15 */
    idx_t k = (n - 1) / 15;                /* number of completed cycles */

    /* a(n) = a(r) + 39 * k */
    return base[r - 1] + ((seq_t)k * 39);
}
