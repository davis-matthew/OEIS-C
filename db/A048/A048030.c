/*
 * A048030  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{⌊n/4⌋} [t ≤ ⌊(n-3)/3⌋ and 3t ≤ n-⌊(n-3)/3⌋] ·
 *            C(m, t) · C(n-m, 3t), where m = ⌊(n-3)/3⌋
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiplicative binomial coefficient C(n,k) using seq_t arithmetic */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i;  all operations are integer */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                           /* offset 1: first term is at n = 1 */
    idx_t m = (n - 3) / 3;                         /* floor((n-3)/3) -- n ≥ 4 guarantees non‑negative */
    idx_t L = n - m;                              /* elements > m */
    idx_t max_t = n / 4;                           /* t = |S| / 4, minimal non‑empty subset */
    seq_t total = 0;

    for (idx_t t = 1; t <= max_t; ++t)
    {
        if (t > m) break;             /* not enough small elements */
        if (t > L / 3) break;         /* not enough large elements */

        seq_t c1 = _binom(m, t);
        seq_t c2 = _binom(L, 3 * t);
        total += c1 * c2;
    }
    return total;
}
