/*
 * A047173  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{t=1}^{\min(\lfloor L/2\rfloor, R, \lfloor
 *            n/3\rfloor)} \binom{L}{2t} \cdot \binom{R}{t} where L = \lfloor
 *            (n-1)/2\rfloor and R = n - L. This counts non‑empty subsets of
 *            {1,…,n} in which exactly 2/3 of the elements are \le (n-1)/2
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient (works for unsigned integer types
   and arbitrary‑precision integer types supporting +,-,*,/). */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* compute L = floor((n-1)/2) and R = n - L */
    idx_t m = (n - 1) / 2;
    idx_t L = m;
    idx_t R = n - m;

    /* maximum t allowed by constraints */
    idx_t tmax1 = L / 2;           /* 2t <= L */
    idx_t tmax2 = R;               /* t <= R */
    idx_t tmax3 = n / 3;           /* 3t <= n */
    idx_t Tmax = tmax1;
    if (tmax2 < Tmax) Tmax = tmax2;
    if (tmax3 < Tmax) Tmax = tmax3;

    seq_t sum = 0;
    for (idx_t t = 1; t <= Tmax; ++t) {
        seq_t left  = binomial(L, 2 * t);
        seq_t right = binomial(R, t);
        sum += left * right;
    }
    return sum;
}
