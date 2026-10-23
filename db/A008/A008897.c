/*
 * A008897  x->x/2 if x even, x->3x-1 if x odd.
 * https://oeis.org/A008897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=83; * For n>0: a(n) = a(n-1)/2 if a(n-1) is even * a(n) =
 *            3*a(n-1)-1 if a(n-1) is odd
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)83;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & (seq_t)1) == 0) {
            x = x / 2;
        } else {
            x = 3 * x - 1;
        }
    }
    return x;
}
