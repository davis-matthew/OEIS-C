/*
 * A037683  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1, a(n) = 6*a(n-1) + d_n, where d_n cycles through
 *            1,0,3,2. a(0) = 0 (used only for internal calculation)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against accidental use below offset – return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    /* digits to be used in order: 1, 0, 3, 2, 1, 0, ... */
    static const seq_t digits[4] = {1, 0, 3, 2};

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* multiply previous result by 6 and add the next digit of the cycle */
        result = result * (seq_t)6 + digits[(i - (idx_t)1) & (idx_t)3];
    }
    return result;
}
