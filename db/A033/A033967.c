/*
 * A033967  Trajectory of 1 under map n->21n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = a(n–1)/2 if a(n–1) is even,
 *            otherwise a(n) = 21·a(n–1) + 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0)
            x = x / 2;
        else
            x = x * 21 + 1;
    }
    return x;
}
