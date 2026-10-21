/*
 * A210606  Length of the n-th edge of an L-toothpick structure which gives
 *          Recamán's sequence A005132.
 * https://oeis.org/A210606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor((n+5)/2) for n ≥ 10. For 1 ≤ n ≤ 9 the values are:
 *            1, 3, 5, 3, 4, 4, 5, 11, 13
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10) {
        /* index 0 is unused; a(0) is not part of the sequence */
        static const seq_t table[10] = {0, 1, 3, 5, 3, 4, 4, 5, 11, 13};
        if (n == 0) return 0;   /* safety, should not happen */
        return table[n];
    }

    /* For n ≥ 10, a(n) = floor((n+5)/2). */
    seq_t res = n / 2;          /* integer division, truncating */
    res += (n & 1) ? 3 : 2;     /* add 3 if n is odd, 2 if even */
    return res;
}
