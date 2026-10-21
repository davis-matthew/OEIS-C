/*
 * A053190  a(n) is the numerator of the n-th convergent of [L_1, L_2, L_3,
 *          ...] where L_i is the period of the continued fraction for
 *          sqrt(i).
 * https://oeis.org/A053190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            [L_1, L_2, L_3, ...] where L_k is the period length of the
 *            continued fraction for sqrt(k). The recurrence used is p_(-1)=1,
 *            p_0=L_1, p_k = L_{k+1}*p_{k-1} + p_{k-2} (for k≥1), and the
 *            answer is p_{n-1}
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit values.  */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    for (int i = 62; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        idx_t next = r + bit;
        /* next*next <= n  <=>  next <= n / next  (avoids overflow)  */
        if (next <= n / next)
            r = next;
    }
    return r;
}

/* Test whether a number is a perfect square. */
static inline int is_square(idx_t n)
{
    idx_t r = isqrt(n);
    return r * r == n;
}

/* Length of the period of the continued fraction of sqrt(D).  */
static inline idx_t period_length(idx_t D)
{
    if (is_square(D))
        return 0;            /* exact square, period length 0 */

    idx_t a0 = isqrt(D);
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    idx_t count = 0;

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        ++count;
    } while (a != 2 * a0);

    return count;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* not part of the sequence, handled safely */

    /* p_{-1} = 1,  p_0 = L1  */
    seq_t prev2 = 1;
    seq_t prev1 = period_length(1);   /* L_1 = 0 */

    if (n == 1)
        return prev1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t Li = period_length(i);   /* L_i */
        seq_t cur = Li * prev1 + prev2;/* p_{i-1} */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;                     /* numerator of the n-th convergent */
}
