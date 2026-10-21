/*
 * A087431  a(n) = 0^n/2 + 2^n*(n^2+n+2)/4.
 * https://oeis.org/A087431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n/2 + 2^n*(n^2+n+2)/4
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the special cases n = 0 and n = 1 */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* For n >= 2 the formula simplifies to
       a(n) = 2^(n-2) * (n^2 + n + 2)  */
    seq_t pow2_n_minus_2 = pow2(n - 2);

    /* Compute n^2 + n + 2 in seq_t, using unsigned overflow if seq_t is
       an unsigned builtin type; for arbitrary‑precision seq_t this is the
       exact value. */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    seq_t poly = n_sq + n_seq + (seq_t)2;

    return pow2_n_minus_2 * poly;
}
