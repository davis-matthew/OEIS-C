/*
 * A188789  Number of 6-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18584*n^4 - 82552*n^3 + 119616*n^2 - 64320*n + 9984 for n
 *            > 4 a(1) = 0, a(2) = 4224, a(3) = 174720, a(4) = 1141248 This
 *            computes OEIS A188789, the number of 6‑step self‑avoiding walks
 *            on an n×n×n×n 4‑cube summed over all starting positions
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 23, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        static const seq_t v[4] = {
            (seq_t)0,
            (seq_t)4224,
            (seq_t)174720,
            (seq_t)1141248
        };
        return v[n-1];
    }

    seq_t nt = (seq_t)n;
    seq_t t = (seq_t)18584;           // a
    t = t * nt - (seq_t)82552;        // a*n + b
    t = t * nt + (seq_t)119616;       // (a*n + b)*n + c
    t = t * nt - (seq_t)64320;        // ... + d
    t = t * nt + (seq_t)9984;         // ... + e
    return t;
}
