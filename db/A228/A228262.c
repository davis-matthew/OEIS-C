/*
 * A228262  Number of third differences of arrays of length 6 of numbers in
 *          0..n.
 * https://oeis.org/A228262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 112*n^3 - 34*n^2 - 38*n - 13 for n>4 * Initial values:
 *            a(1)=63, a(2)=621, a(3)=2511, a(4)=6419
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 128, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)63;
        case 2: return (seq_t)621;
        case 3: return (seq_t)2511;
        case 4: return (seq_t)6419;
        default: {
            seq_t nn = (seq_t)n;
            /* Compute 112*n^3 - 34*n^2 - 38*n - 13 */
            seq_t n2 = nn * nn;
            seq_t n3 = n2 * nn;
            seq_t res = 112 * n3;
            res -= 34 * n2;
            res -= 38 * nn;
            res -= (seq_t)13;
            return res;
        }
    }
}
