/*
 * A000166  Subfactorial or rencontres numbers, or derangements: number of
 *          permutations of n elements with no fixed points.
 * https://oeis.org/A000166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) + (-1)^n, a(0) = 1
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
    for (i = 1; i <= n; i++) {
        r = r * (seq_t)i;
        if (i & 1)
            r -= 1;
        else
            r += 1;
    }
    return r;
}
