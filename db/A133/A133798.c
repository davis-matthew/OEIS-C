/*
 * A133798  a(n) = A002467(n) - 1.
 * https://oeis.org/A133798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; * For n>1: * if n is odd: a(n) = n * (a(n-1) + 1); * if
 *            n is even: a(n) = n * (a(n-1) + 1) - 2. * This recursive
 *            definition produces a(n)=A002467(n)-1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t prev = 0;       /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t term = prev + 1;          /* a(k-1)+1 */
        if (k & 1U) {                   /* odd k */
            prev = (seq_t)k * term;
        } else {                         /* even k */
            prev = (seq_t)k * term - 2;
        }
    }
    return prev;
}
