/*
 * A033977  Trajectory of 1 under map n->43n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = (a(n-1) even ? a(n-1)/2 : 43*a(n-1)+1)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 47, u64 n <= 69, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 1;
    for (idx_t i = 0; i < n; ++i) {
        if ((v % 2) == 0)
            v = v / 2;
        else
            v = v * 43 + 1;
    }
    return v;
}
