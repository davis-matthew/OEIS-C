/*
 * A028673  Galois numbers for p=11; order of group AGL(n,11).
 * https://oeis.org/A028673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11^n * Product_{k=0..n-1} (11^n - 11^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res   = (seq_t)1;
    seq_t pow11 = (seq_t)11;

    for (idx_t j = 1; j <= n; ++j) {
        res *= pow11;                       /* multiply by 11^j                 */
        seq_t t = pow11 - (seq_t)1;         /* 11^j - 1                         */
        res *= t;
        pow11 *= (seq_t)11;                  /* next power: 11^{j+1}             */
    }

    return res;
}
