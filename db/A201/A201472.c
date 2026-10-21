/*
 * A201472  The Griesmer lower bound q_4(5,n) on the length of a linear code
 *          over GF(4) of dimension 5 and minimal distance n.
 * https://oeis.org/A201472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..4} ceil(n / 4^i) (Griesmer lower bound for GF(4)
 *            linear code of dimension 5)
 * Range:     u8 none, u16 n <= 100, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to the arithmetic type */
    seq_t res = 0;
    seq_t d = 1;                 /* 4^i for i = 0..4 */

    for (int i = 0; i < 5; ++i) {
        /* ceil(N / d) = (N + d - 1) / d, works for all unsigned seq_t types */
        seq_t term = (N + d - 1) / d;
        res += term;
        d *= 4;                  /* next power of 4 */
    }

    return res;
}
