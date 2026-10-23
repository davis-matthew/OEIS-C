/*
 * A130835  Sum of all numbers having n or fewer digits and having the sum of
 *          their digits equal to n.
 * https://oeis.org/A130835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ((10^n-1)/9) * [x^n] ((x^10-1)/(x-1))^n = ((10^n-1)/9) *
 *            sum_{k=0}^{⌊n/10⌋} (-1)^k * C(n, k) * C(n + (n-10k) -1, n-10k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Forward declaration of helper: compute binomial coefficient C(N, K) as seq_t */
static seq_t binom_nk(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t mult = (seq_t)(N - K + i);
        res = res * mult;
        res = res / (seq_t)i;          /* division is exact */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1; return 0 */

    /* Compute factor ((10^n-1)/9) */
    seq_t pow10n = pow10(n);
    seq_t factor = (pow10n - 1) / 9;

    /* Compute coefficient c_n = sum_{k} (-1)^k * C(n,k) * C(n + m -1, m), m = n-10k */
    seq_t pos = 0;   /* sum of positive terms */
    seq_t neg = 0;   /* sum of negative terms */
    for (idx_t k = 0; k <= n / 10; ++k) {
        idx_t m = n - 10 * k;
        seq_t c1 = binom_nk(n, k);
        seq_t c2 = binom_nk(n + m - 1, m);
        seq_t term = c1 * c2;
        if (k & 1)   /* k odd => negative */
            neg += term;
        else        /* k even => positive */
            pos += term;
    }
    seq_t coef = pos - neg;          /* pos >= neg by combinatorial identity */

    /* Final answer */
    return factor * coef;
}
