/*
 * A033963  Trajectory of 1 under map n->11n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n+1)=f(a(n)) where f(x) = (x is odd) ? 11*x+1 : x/2
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A033963: trajectory of 1 under the map
   n -> 11*n+1 if n odd, n -> n/2 if n even. */
seq_t a(idx_t n)
{
    seq_t x = 1;
    for (idx_t i = 0; i < n; ++i) {
        if (x & ((seq_t)1)) {          /* odd */
            x = x * (seq_t)11 + (seq_t)1;
        } else {                       /* even */
            x = x >> 1;                /* equivalent to x/2 */
        }
    }
    return x;
}
