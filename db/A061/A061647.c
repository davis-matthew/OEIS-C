/*
 * A061647  Beginning at the well for the topograph of a positive definite
 *          quadratic form with values 1, 1, 1 at a superbase (i.e., 1, 1 and
 *          1 are the vonorms of the superbase), these numbers indicate the
 *          labels of the edges of the topograph on a path of greatest ascent.
 * https://oeis.org/A061647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with a(1)=1, a(2)=3, a(3)=9
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    seq_t a0 = 1;          /* a(1) */
    if (n == 1) return a0;
    seq_t a1 = 3;          /* a(2) */
    if (n == 2) return a1;
    seq_t a2 = 9;          /* a(3) */
    if (n == 3) return a2;

    /* Iteratively compute the sequence using the recurrence. */
    seq_t curr;
    for (idx_t i = 4; i <= n; ++i) {
        curr = a2 + a2 + a1 + a1 - a0;   /* 2*a2 + 2*a1 - a0 */
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
