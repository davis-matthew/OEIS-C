/*
 * A123337  Number of ordered ways to write n as the sum of 5 squares less
 *          than 5^2.
 * https://oeis.org/A123337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = # of ordered 5‑tuples (x1,…,x5) with 0≤xi≤4 and ∑xi² = n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t sq[5] = {0, 1, 4, 9, 16};
    seq_t N = (seq_t)n;
    seq_t count = 0;
    for (idx_t a = 0; a < 5; ++a) {
        seq_t sa = sq[a];
        for (idx_t b = 0; b < 5; ++b) {
            seq_t sb = sa + sq[b];
            for (idx_t c = 0; c < 5; ++c) {
                seq_t sc = sb + sq[c];
                for (idx_t d = 0; d < 5; ++d) {
                    seq_t sd = sc + sq[d];
                    if (sd > N) continue;
                    seq_t rem = N - sd;
                    for (idx_t e = 0; e < 5; ++e) {
                        if (rem == sq[e]) {
                            ++count;
                        }
                    }
                }
            }
        }
    }
    return count;
}
