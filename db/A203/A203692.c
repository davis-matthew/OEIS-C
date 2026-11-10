/*
 * A203692  v(n+1)/v(n), where v=A203691.
 * https://oeis.org/A203692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (T_i^2 + T_i*T_{n+1} + T_{n+1}^2), T_k =
 *            k(k+1)/2 (i.e. v(n+1)/v(n), v=A203691)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;
    seq_t tm = m * (m + 1) / 2;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ti = (seq_t)i * ((seq_t)i + 1) / 2;
        result *= ti * ti + ti * tm + tm * tm;
    }
    return result;
}
