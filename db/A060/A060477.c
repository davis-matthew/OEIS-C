/*
 * A060477  Number of orbits of length n in map whose periodic points are
 *          A000051.
 * https://oeis.org/A060477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} μ(d) * (2^{n/d} + 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 1, a(3) = 2, a(4) = 3, ... */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, so this case is unused */

    seq_t sum_pos = 0, sum_neg = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* First divisor d = i */
            idx_t d1 = i;
            idx_t div1 = n / d1;                 /* n/d1 */
            seq_t pow1 = big_pow((seq_t)2, (seq_t)div1);
            int mu1 = mu_of(d1);
            if (mu1 == 1) sum_pos += pow1;
            else if (mu1 == -1) sum_neg += pow1;

            /* Second divisor d = n/i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t div2 = n / d2;             /* n/d2 = i */
                seq_t pow2 = big_pow((seq_t)2, (seq_t)div2);
                int mu2 = mu_of(d2);
                if (mu2 == 1) sum_pos += pow2;
                else if (mu2 == -1) sum_neg += pow2;
            }
        }
    }

    seq_t sum = sum_pos - sum_neg;
    if (n == 1) sum += 1;          /* add the constant term for n = 1 */

    return sum / (seq_t)n;
}
