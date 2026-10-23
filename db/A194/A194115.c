/*
 * A194115  Sum{floor(j*sqrt(12)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(12).
 * https://oeis.org/A194115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1}^{n} floor(j*sqrt(12))
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = Sum_{j=1}^{n} floor(sqrt(12*j*j))     */

#include <stdint.h>

/* Integer square‑root: greatest integer r with r*r <= x.
   Works for any seq_t that supports +, -, *, /, <=. */
static seq_t
isqrt(seq_t x)
{
    seq_t low = 0, high = x;
    /* If x is zero, return 0 immediately. */
    if (high == 0U) return 0U;
    while (low < high) {
        seq_t mid = (low + high + 1U) / 2U;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1U;
    }
    return low;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0U) return 0U;            /* safe guard for n < 1 */
    seq_t sum = 0U;
    for (idx_t j = 1U; j <= n; ++j) {
        seq_t jj   = (seq_t)j;         /* cast to seq_t once */
        seq_t prod = (seq_t)12U * jj * jj;
        seq_t term = isqrt(prod);
        sum += term;
    }
    return sum;
}
