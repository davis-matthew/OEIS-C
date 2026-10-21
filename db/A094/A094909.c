/*
 * A094909  Let p_k(n) = number of partitions of n into exactly k parts;
 *          sequence gives p_3(n-3) + p_2(n-2) + 1.
 * https://oeis.org/A094909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p₃(n‑3) + p₂(n‑2) + 1 where p₃(m) = ⌊(m² + 3)/12⌋ for m ≥
 *            0 (number of partitions of m into 3 parts), and p₂(m) = ⌊m/2⌋
 *            for m ≥ 0 (number of partitions of m into 2 parts)
 * Range:     u8 n <= 18, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t part2 = 0;
    seq_t part3 = 0;

    if (n >= 2) {
        idx_t m2 = n - 2;
        part2 = (seq_t)m2 / (seq_t)2;
    }
    if (n >= 3) {
        idx_t m3 = n - 3;
        seq_t m3_sq = (seq_t)m3 * (seq_t)m3;
        part3 = (m3_sq + (seq_t)3) / (seq_t)12;
    }
    return part2 + part3 + (seq_t)1;
}
