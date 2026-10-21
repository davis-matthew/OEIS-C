/*
 * A079165  a(n) = (4n-2)*a(n-1)+a(n-2) with a(0)=1 and a(1)=2.
 * https://oeis.org/A079165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n) = (4n‑2)*a(n‑1)+a(n‑2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)2; /* a(n-1) */
    seq_t a_n   = 0;       /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 4*i - 2  (use seq_t to avoid overflow on i) */
        seq_t factor = (seq_t)4 * (seq_t)i - (seq_t)2;
        a_n = factor * a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
