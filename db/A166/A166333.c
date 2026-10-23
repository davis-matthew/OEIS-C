/*
 * A166333  The largest prime that divides A027642(n) (the denominator of the
 *          Bernoulli number B_n), or 1 if A027642(n) is 1.
 * https://oeis.org/A166333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that (p-1) divides n, or 1 if no
 *            such prime (i.e. n odd or n = 0, where the denominator of B_n is
 *            1)
 * Range:     u8 n <= 255, u16 n <= 8403, u32 n <= 8403, u64 n <= 8403,
 *            u128 n <= 8403, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

/* Computes a(n) for any n >= 0, with external linkage. */
seq_t a(idx_t n)
{
    /* Special cases for n = 0 and n = 1 */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* For odd n (>1), the denominator of B_n is 1 */
    if ((n & (idx_t)1) != 0) return (seq_t)1;

    /* Even n >= 2: find the largest prime p with p-1 | n.
       Since 1 | n, p = 2 is always present. */
    idx_t maxp = (idx_t)2;                /* 2 is the smallest candidate */
    idx_t limit = int_sqrt(n);            /* integer sqrt of n */

    for (idx_t i = (idx_t)1; i <= limit; ++i) {
        if (n % i == (idx_t)0) {
            /* First divisor */
            idx_t d1 = i;
            idx_t candidate1 = d1 + (idx_t)1;
            if (is_prime(candidate1) && candidate1 > maxp) {
                maxp = candidate1;
            }

            /* Complement divisor */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t candidate2 = d2 + (idx_t)1;
                if (is_prime(candidate2) && candidate2 > maxp) {
                    maxp = candidate2;
                }
            }
        }
    }

    return (seq_t)maxp;
}
