/*
 * A234904  a(n)*Pi is the total length of irregular spiral (center points: 2,
 *          1, 3) after n rotations.
 * https://oeis.org/A234904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n if n mod 3 ≠ 1, otherwise a(n) = 6*n – 3
 * Range:     u8 n <= 43, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A234904 directly without memoisation. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* below offset, undefined */

    seq_t res = (seq_t)n * 6;
    if ((n % 3) == 1)
        res -= 3;

    return res;
}
