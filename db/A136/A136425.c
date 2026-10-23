/*
 * A136425  a(n) = floor((x^n-(1-x)^n)/sqrt(7)+1/2) where x = (sqrt(7)+1)/2.
 * https://oeis.org/A136425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((x^n-(1-x)^n)/sqrt(7)+1/2) where x = (sqrt(7)+1)/2.
 *            The sequence satisfies M1 = 2, M2 = 4, Mn = 2·M_{n-1} +
 *            6·M_{n-2} (n ≥ 3), where M_n = 2^n · (x^n-(1-x)^n)/sqrt(7). Then
 *            a(n) = floor((M_n + 2^{n-1}) / 2^n). The recurrence is computed
 *            in O(n) integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n <= 2) {
        return (seq_t)1;   /* a(1)=1, a(2)=1 */
    }

    /* Initialize recurrence for M_n */
    seq_t prev2 = (seq_t)2;  /* M1 */
    seq_t prev1 = (seq_t)4;  /* M2 */
    seq_t cur   = prev1;     /* placeholder for M_n */

    /* Compute M_n by the linear recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)2 + prev2 * (seq_t)6;
        prev2 = prev1;
        prev1 = cur;
    }

    /* a(n) = floor((M_n + 2^{n-1}) / 2^n) */
    seq_t pow2_n      = pow2(n);          /* 2^n  */
    seq_t pow2_n_minus = pow2(n - 1);      /* 2^{n-1} */
    seq_t sum = cur + pow2_n_minus;
    return sum / pow2_n;
}
