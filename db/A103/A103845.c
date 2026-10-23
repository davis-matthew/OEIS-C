/*
 * A103845  Product of first n Lucas numbers, plus one.
 * https://oeis.org/A103845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + ∏_{k=1..n} A000204(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A103845:
   a(0) = 1,
   a(n) = 1 + product of the first n Lucas numbers (L1 = 1, L2 = 3, L3 = 4, …). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;            /* empty product defined as 0 → 1 + 0 = 1 */

    seq_t prod   = 1;          /* product of Lucas numbers encountered so far */
    seq_t L_prev = 2;          /* Lucas number L0 */
    seq_t L_cur  = 1;          /* Lucas number L1 */

    for (idx_t i = 1; i <= n; ++i) {
        prod *= L_cur;                     /* include current Lucas number */
        seq_t L_next = L_cur + L_prev;     /* compute L_{i+1} = L_i + L_{i-1} */
        L_prev = L_cur;
        L_cur  = L_next;
    }

    return prod + 1;       /* final term a(n) */
}
