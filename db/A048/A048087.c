/*
 * A048087  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{min(L,⌊H/4⌋)} C(L,m) · C(H,4m), where L =
 *            ⌊(n+3)/3⌋ and H = n − L. This counts the number of non‑empty
 *            subsets of {1,…,n} in which exactly one fifth of the elements
 *            are ≤ (n+3)/3
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the binomial coefficient C(n,k) by iterative multiplication
   and division, working entirely with seq_t arithmetic.  Assumes 0≤k≤n.
   For k>n the result is 0.  The algorithm keeps the intermediate value
   integral, but may overflow a fixed‑width seq_t; overflow is
   acceptable for the 64‑bit build when the true value does not fit. */
static seq_t binom_helper(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* L = ⌊(n+3)/3⌋, H = n − L */
    idx_t L = (n + 3) / 3;
    idx_t H = n - L;

    /* Upper bound for m: m ≤ L and 4m ≤ H */
    idx_t M = L;
    idx_t H_div4 = H / 4;
    if (H_div4 < M) M = H_div4;

    seq_t result = 0;
    for (idx_t m = 1; m <= M; ++m) {
        seq_t low  = binom_helper(L, m);          /* C(L, m)   */
        seq_t high = binom_helper(H, 4 * m);       /* C(H, 4m)  */
        result += low * high;
    }
    return result;
}
