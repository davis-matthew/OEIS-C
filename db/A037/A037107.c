/*
 * A037107  Trajectory of 3 under map n->19n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map-based definition of A037107:  a(0)=3; for n>0, a(n)=  a(n-1)/2 if a(n-1) even, else 19*a(n-1)+1 */
seq_t a(idx_t n)
{
    seq_t x = 3;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0) {
            x = x / 2;
        } else {
            x = x * 19 + 1;
        }
    }
    return x;
}
