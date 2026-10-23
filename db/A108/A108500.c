/*
 * A108500  Number of values of k (1<=k<=n) where k^(n+1) != k mod n, or
 *          equivalently where sum_i{1<=i<=n} k^i != 0 mod n.
 * https://oeis.org/A108500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (# of k in [1,n] such that k^(n+1) ≡ k (mod n))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here we compute the number of k where the sum k^1+...+k^n ≡ 0 (mod n). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special handling for n=0 is not required, as the offset is 1. */
    idx_t solutions = 0;              /* Count of k satisfying the congruence. */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t sum  = 0;
        idx_t prod = 1 % n;            /* prod = k^i (mod n) for the current i. */
        for (idx_t i = 1; i <= n; ++i)
        {
            prod = (prod * k) % n;    /* Compute k^i mod n. */
            sum  += prod;
            if (sum >= n) sum -= n;    /* Keep sum mod n. */
        }
        if (sum == 0)
            ++solutions;
    }
    return (seq_t)(n - solutions);
}
