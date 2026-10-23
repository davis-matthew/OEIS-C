/*
 * A010952  Binomial coefficient C(36,n).
 * https://oeis.org/A010952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(36,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 36)
        return 0;
    idx_t k = n;
    if (k > 36 - k)
        k = 36 - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(36 - k + i)) / ((seq_t)i);
    }
    return res;
}
