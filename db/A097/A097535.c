/*
 * A097535  Dimensions of spaces of cusp forms.
 * https://oeis.org/A097535
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+24m) = a(n) + 12*m for m>=1. Initial values for n=4..27:
 *            a(4)=1, a(5)=1, a(6)=1, a(7)=2, a(8)=2, a(9)=3, a(10)=4,
 *            a(11)=4, a(12)=4, a(13)=5, a(14)=5, a(15)=5, a(16)=6, a(17)=7,
 *            a(18)=7, a(19)=8, a(20)=8, a(21)=8, a(22)=9, a(23)=9, a(24)=9,
 *            a(25)=12, a(26)=12, a(27)=12
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[24] = {
    1, 1, 1, 2, 2, 3, 4, 4, 4, 5, 5, 5,
    6, 7, 7, 8, 8, 8, 9, 9, 9, 12, 12, 12
};
seq_t a(idx_t n)
{
    if (n < 4) return 0;  /* undefined for n<4; caller guarantees n>=4 */

    idx_t offset = n - 4;           /* distance from the first known value */
    idx_t q       = offset / 24;    /* number of 24‑step jumps */
    idx_t r       = (offset % 24) + 4; /* resulting index in [4,27] */

    return base[r - 4] + (seq_t)12 * (seq_t)q;
}
