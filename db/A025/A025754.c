/*
 * A025754  9th-order Patalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9th‑order Patalan numbers (A025754)
    a(0) = 1
    a(1) = 1
    for n ≥ 2:  a(n) = a(n‑1) * (81*n – 90) / n
*/

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = ((seq_t)81 * ((seq_t)i)) - (seq_t)90;
        res = (res * factor) / i;
    }
    return res;
}
