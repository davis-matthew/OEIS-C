/*
 * A047242  Numbers that are congruent to {0, 1, 3} mod 6.
 * https://oeis.org/A047242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + floor((n-1)/3) + floor((2n-2)/3)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) = 0, a(2) = 1, a(3) = 3, ...  */
    if (n == 0) return 0;          /* safety for indices below the offset */
    seq_t t = (seq_t)n - 1;         /* t = n-1, all arithmetic is in seq_t */
    seq_t t_div3 = t / 3;           /* floor((n-1)/3)   */
    seq_t t_rem  = t % 3;           /* remainder of t mod 3   */
    /* floor((2t)/3) without overflow: (t/3)*2 + (t%3==2 ? 1 : 0) */
    seq_t two_third = t_div3 * 2 + (t_rem == 2 ? 1 : 0);
    return t + t_div3 + two_third;
}
