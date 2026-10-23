/*
 * A187166  Number of 5-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 726*n^3 - 2640*n^2 + 2688*n - 720 for n >= 4, with
 *            a(1)=0, a(2)=144, a(3)=3240
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 0;
        case 1: return 0;
        case 2: return 144;
        case 3: return 3240;
        default: {
            seq_t nn = (seq_t)n;
            seq_t res = 726ULL * nn * nn * nn;
            res -= 2640ULL * nn * nn;
            res += 2688ULL * nn;
            res -= 720ULL;
            return res;
        }
    }
}
