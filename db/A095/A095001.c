/*
 * A095001  a(n) = Sum_{d|n} rad(d)^(n/d), where rad(d) = A007947(d) is the
 *          squarefree kernel of d.
 * https://oeis.org/A095001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} rad(d)^(n/d) where rad(d) is the * squarefree
 *            kernel of d. For this implementation we * enumerate the divisors
 *            of n in O(√n) time, obtain rad(d) * with the helper function
 *            rad(idx_t), and compute the * power via repeated multiplication
 *            in O(exp) steps. This * method is fast enoug...
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 59, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"       /* seq_t rad(idx_t m); */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t rad1 = rad(d1);
            sum += pow_seq_t(rad1, n / d1);

            if (d2 != d1) {
                seq_t rad2 = rad(d2);
                sum += pow_seq_t(rad2, n / d2);
            }
        }
    }
    return sum;
}
