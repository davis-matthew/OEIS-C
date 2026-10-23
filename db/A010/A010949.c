/*
 * A010949  Binomial coefficient C(33,n).
 * https://oeis.org/A010949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(33, n) = 33! / (n! (33-n)!)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 33) return (seq_t)0;
    idx_t k = n;
    if (k > 33 - k) k = 33 - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)33 - k + i);
        res = res / (seq_t)i;
    }
    return res;
}
