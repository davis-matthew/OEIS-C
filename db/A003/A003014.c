/*
 * A003014  Expansion of e.g.f.: 1 + x*exp(x) + x^2*exp(2*x) + x^3*exp(3*x).
 * https://oeis.org/A003014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + n*(n-1)*2^(n-2) + n*(n-1)*(n-2)*3^(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   a003014_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003014_pw.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t result = (seq_t)n;
    if (n >= 2) {
        seq_t term = (seq_t)n * ((seq_t)n - 1) * a003014_pw((seq_t)2, n - 2);
        result = result + term;
    }
    if (n >= 3) {
        seq_t term = (seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2) * a003014_pw((seq_t)3, n - 3);
        result = result + term;
    }
    return result;
}
