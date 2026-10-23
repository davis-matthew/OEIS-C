/*
 * A138985  a(n) = Frobenius number for 5 successive numbers = F(n+1, n+2,
 *          n+3, n+4, n+5).
 * https://oeis.org/A138985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(n‑1)/4⌋·(n+1) + n (Frobenius number for n+1 … n+5)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1.  For safety we return 0 for n == 0. */
    if (n == 0) return 0;

    /* m = n+1 is the smallest element of the set {m, m+1, …, m+4} */
    seq_t m = (seq_t)(n + 1);

    /* q = floor((n-1)/4) ≡ floor((m-2)/4) */
    seq_t q = (seq_t)((n - 1) / 4);

    /* a(n) = q·m + n */
    return q * m + n;
}
