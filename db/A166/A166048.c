/*
 * A166048  a(n) = (A166049(n)-1)/4.
 * https://oeis.org/A166048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A166049(n)-1)/4; for n ≥ 14 this simplifies to
 *            (n-8)*(n-7)
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n: use pre‑computed values */
    if (n <= 14) {
        static const seq_t pre[] = {
            0, 1, 2, 3, 5, 6, 9, 12, 20, 21, 23, 30, 41, 42
        };
        return pre[n - 1];
    }
    /* n ≥ 15: closed form */
    seq_t m = (seq_t)(n - 8);
    return m * ((seq_t)(n - 7));
}
