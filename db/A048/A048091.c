/*
 * A048091  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{t\ge1} \binom{\lfloor (n+3)/3\rfloor}{t} \;
 *            \binom{\,n-\lfloor (n+3)/3\rfloor\,}{5t}
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t l = (n + 3) / 3;          /* number of elements <= (n+3)/3 */
    idx_t r = n - l;                /* number of elements > (n+3)/3 */

    /* t must satisfy t ≤ l, 5·t ≤ r, and 6·t ≤ n */
    idx_t t_max = l;
    if (r / 5 < t_max) t_max = r / 5;
    if (n / 6 < t_max) t_max = n / 6;
    if (t_max == 0) return 0;

    seq_t ans = 0;
    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t c1 = binomial(l, t);
        seq_t c2 = binomial(r, 5 * t);
        ans += c1 * c2;
    }
    return ans;
}
