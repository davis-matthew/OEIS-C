/*
 * A133260  Unknown.
 * https://oeis.org/A133260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn
 * Formula:   a(n) = 20 + 2 * Σ_{k=1}^{n-1} (10*⌊(k-1)/3⌋ + {1,4,7}[(k-1) mod
 *            3])
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)20;
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t r = (k - 1) % 3;
        seq_t base = (seq_t)10 * ((k - 1) / 3);
        seq_t val;
        if (r == 0)
            val = base + (seq_t)1;
        else if (r == 1)
            val = base + (seq_t)4;
        else
            val = base + (seq_t)7;
        sum += val * (seq_t)2;
    }
    return sum + (seq_t)20;
}
