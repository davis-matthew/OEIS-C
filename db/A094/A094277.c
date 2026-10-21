/*
 * A094277  First column of triangle A094275.
 * https://oeis.org/A094277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094277: First column of triangle A094275.
   Formula: a(n) = n*(n-3)/2 + 7   for 9 ≤ n < 47
            a(n) = n*(n-3)/2 + 8   for n ≥ 47
            a(1..8) are the given initial terms. */
seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 2, 4, 7, 11, 15, 20, 26};
    if (n <= 8)
        return init[n - 1];

    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn - 3) / 2;
    if (n < 47)
        val += (seq_t)7;
    else
        val += (seq_t)8;
    return val;
}
