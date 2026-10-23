/*
 * A135910  Number of triples (x,y,z) of nonnegative integers such that
 *          x^2+y^3+z^4 = n.
 * https://oeis.org/A135910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z) ∈ ℕ₀³ : x² + y³ + z⁴ = n}
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t z = 0; ; ++z) {
        seq_t zz = (seq_t)z;
        seq_t z2 = zz * zz;
        seq_t z4 = z2 * z2;
        if (z4 > (seq_t)n) break;
        seq_t rem1 = (seq_t)n - z4;
        for (idx_t y = 0; ; ++y) {
            seq_t yy = (seq_t)y;
            seq_t y2 = yy * yy;
            seq_t y3 = y2 * yy;
            if (y3 > rem1) break;
            seq_t rem2 = rem1 - y3;
            idx_t rem_tmp = (idx_t)rem2;
            idx_t s = 0;
            while ((s + 1) * (s + 1) <= rem_tmp) ++s;
            seq_t sseq = (seq_t)s;
            if (sseq * sseq == rem2) ++count;
        }
    }
    return count;
}
