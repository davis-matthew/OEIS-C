/*
 * A000240  Rencontres numbers: number of permutations of [n] with exactly one
 *          fixed point.
 * https://oeis.org/A000240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) - (-1)^n*n, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    for (i = 2; i <= n; i++) {
        r = r * (seq_t)i;
        if (i & 1)
            r += (seq_t)i;
        else
            r -= (seq_t)i;
    }
    return r;
}
