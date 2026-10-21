/*
 * A056665  Number of equivalence classes of n-valued Post functions of 1
 *          variable under action of complementing group C(1,n).
 * https://oeis.org/A056665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * n^{n/d-1}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = (seq_t)0;
    seq_t n_seq = (seq_t)n;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t phi_d = calc_phi((seq_t)d);
            idx_t div = n / d;          /* n/d ≥ 1 */
            seq_t pow_val = pow_seq(n_seq, div - 1); /* n^{n/d-1} */
            sum += phi_d * pow_val;
        }
    }
    return sum;
}
