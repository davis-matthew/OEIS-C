/*
 * A014818  a(n) is the sum over all floor(k^3/n), k=0 to n inclusive.
 * https://oeis.org/A014818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} ⌊k^3 / n⌋
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1878, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset starts at 1; protect against divide‑by‑zero

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) /* k runs from 0 to n inclusive */
    {
        seq_t kseq = k;
        sum += (kseq * kseq * kseq) / (seq_t)n;  /* integer division = ⌊k^3 / n⌋ */
    }
    return sum;
}
