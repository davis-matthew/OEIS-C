/*
 * A194764  Number of k such that {-k*2^(1/3)} < {-n*2^(1/3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k with 0 ≤ k ≤ n such that *
 *            {-k*2^(1/3)} < {-n*2^(1/3)}. * This is equivalent to counting k
 *            = 0 together with all * 1 ≤ k ≤ n for which the fractional part
 *            {k*α} > {n*α}, * where α = 2^(1/3)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* high‑precision cube root of 2 */
    const long double alpha = cbrtl(2.0L);

    /* fractional part of n*α */
    long double n_prod = (long double)n * alpha;
    long double n_frac = fmodl(n_prod, 1.0L);
    if (n_frac < 0.0L) n_frac += 1.0L;          /* safety for negative values */

    seq_t count = 1;                            /* k = 0 always satisfies */

    for (idx_t k = 1; k <= n; ++k) {
        long double k_prod = (long double)k * alpha;
        long double k_frac = fmodl(k_prod, 1.0L);
        if (k_frac > n_frac) {
            ++count;
        }
    }

    return count;
}
