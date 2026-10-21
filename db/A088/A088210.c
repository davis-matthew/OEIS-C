/*
 * A088210  Numerators of convergents of the continued fraction with the n+1
 *          partial quotients: [2;2,2,...(n 2's)...,2,n+1], starting with [1],
 *          [2;2], [2;2,3], [2;2,2,4], ...
 * https://oeis.org/A088210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = A000129(n) + (n+1)*A000129(n+1) (Pell numbers)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two smallest indices directly. */
    if (n == 0u) {
        /* P(0)=0, P(1)=1 → a(0)=0+1*1=1 */
        return (seq_t)1;
    }
    if (n == 1u) {
        /* P(1)=1, P(2)=2 → a(1)=1+2*2=5 */
        return (seq_t)1 + (seq_t)2 * (seq_t)2;
    }

    /* For n ≥ 2 compute Pell numbers P(n) and P(n+1). */
    seq_t p_im2 = (seq_t)0; /* P(0)   */
    seq_t p_im1 = (seq_t)1; /* P(1)   */
    seq_t p_n   = (seq_t)0; /* P(n)   */
    seq_t p_n1  = (seq_t)0; /* P(n+1) */

    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t p_cur = (seq_t)2 * p_im1 + p_im2;  /* P(i) = 2*P(i‑1)+P(i‑2) */
        if (i == n)   p_n  = p_cur;               /* store P(n)   */
        if (i == n+1) p_n1 = p_cur;               /* store P(n+1) */
        p_im2 = p_im1;
        p_im1 = p_cur;
    }

    seq_t coeff = (seq_t)(n + 1);
    return p_n + coeff * p_n1;
}
