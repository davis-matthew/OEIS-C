/*
 * A121693  Number of deco polyominoes of height n and vertical height 3
 *          (i.e., having 3 rows).
 * https://oeis.org/A121693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(a(n-1) + 2^(n-2) - 1), a(1)=a(2)=0, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 0;
    if (n == 3)
        return 1;

    seq_t result = 1;            /* a(3) */
    seq_t pow2   = 4;            /* 2^(4-2) = 2^2 */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t tmp = result + pow2 - 1;
        tmp *= 3;
        result = tmp;
        if (i < n)
            pow2 <<= 1;         /* multiply by 2 for next 2^(i-1) */
    }
    return result;
}
