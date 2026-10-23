/*
 * A147674  Period 9: repeat 81,27,9,27,27,9,27,81,9.
 * https://oeis.org/A147674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = a(n-9) with initial values a(0)=81, a(1)=27, a(2)=9,
 *            a(3)=27, a(4)=27, a(5)=9, a(6)=27, a(7)=81, a(8)=9
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 9 */
    static const seq_t period[9] = {
        81, /* a(0) */
        27, /* a(1) */
        9,  /* a(2) */
        27, /* a(3) */
        27, /* a(4) */
        9,  /* a(5) */
        27, /* a(6) */
        81, /* a(7) */
        9   /* a(8) */
    };
    return period[n % 9];
}
