/*
 * A183127  Number of toothpicks added at n-th stage to the toothpick
 *          structure of A183126.
 * https://oeis.org/A183127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(1 + 3^A000120(n-2)), n >= 3 a(0)=0, a(1)=1, a(2)=6
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;

    /* binary weight of n-2 */
    idx_t m = n - 2;
    idx_t weight = 0;
    while (m) {
        weight += m & 1;
        m >>= 1;
    }

    seq_t pow3 = 1;
    for (idx_t i = 0; i < weight; ++i) {
        pow3 *= (seq_t)3;
    }

    return 4 * (1 + pow3);
}
