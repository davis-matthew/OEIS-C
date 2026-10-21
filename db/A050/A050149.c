/*
 * A050149  a(n) = T(n,n-3), array T as in A050143.
 * https://oeis.org/A050149
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1..n} binomial(n,s)*binomial(s+n-5,n-4) (n ≥ 4)
 *            Special case: a(3) = 1, values for n < 3 are undefined but
 *            return 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Handle the offset: a(3) is the first term */
    if (n == 3) return (seq_t)1;
    if (n < 3) return (seq_t)0;   /* undefined, but safe return */

    seq_t sum = (seq_t)0;
    for (idx_t s = (idx_t)1; s <= n; ++s) {
        seq_t term = binom(n, s) * binom(s + n - (idx_t)5, n - (idx_t)4);
        sum += term;
    }
    return sum;
}
