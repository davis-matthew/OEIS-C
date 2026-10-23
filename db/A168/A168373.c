/*
 * A168373  a(n) = 7*n - a(n-1) - 6 with n>1, a(1)=4.
 * https://oeis.org/A168373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n - 7*(-1)^n - 5)/4 , n ≥ 1, a(1) = 4
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t numerator;

    if (n & 1)          /* n is odd */
        numerator = (seq_t)14 * n_s + (seq_t)2;
    else                /* n is even */
        numerator = (seq_t)14 * n_s - (seq_t)12;

    /* The numerator is always divisible by 4 */
    return numerator / (seq_t)4;
}
