/*
 * A135911  Number of 4-tuples (x,y,z,t) of nonnegative integers such that
 *          x^2+y^3+z^4+t^5 = n.
 * https://oeis.org/A135911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {(x,y,z,t) ≥ 0 | x² + y³ + z⁴ + t⁵ = n}
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t n_seq  = (seq_t)n;

    /* Compute all 5th powers ≤ n once (very few). */
    seq_t fifths[10];
    int  fifth_cnt = 0;
    for (idx_t t = 0;; ++t) {
        seq_t val = 1;
        for (int k = 0; k < 5; ++k) val *= t;
        if (val > n_seq) break;
        fifths[fifth_cnt++] = val;
    }

    for (idx_t x = 0;; ++x) {
        seq_t sq = (seq_t)x * x;
        if (sq > n_seq) break;
        seq_t rem1 = n_seq - sq;

        for (idx_t y = 0;; ++y) {
            seq_t cube = (seq_t)y * y * y;
            if (cube > rem1) break;
            seq_t rem2 = rem1 - cube;

            for (idx_t z = 0;; ++z) {
                seq_t fourth = (seq_t)z * z * z * z;
                if (fourth > rem2) break;
                seq_t rest = rem2 - fourth;

                /* Check if rest is a 5th power. */
                for (int i = 0; i < fifth_cnt; ++i) {
                    if (fifths[i] == rest) {
                        ++result;
                        break;
                    }
                }
            }
        }
    }

    return result;
}
