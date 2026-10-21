/*
 * A087909  a(n) = Sum_{d|n} (n/d)^(d-1).
 * https://oeis.org/A087909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{d|n} (n/d)^(d-1) (OEIS A087909, offset 1)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 125,
 *            u128 n <= 245, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>   /* for malloc / realloc, used only for type safety */

/* Helper: fast exponentiation for seq_t values. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {            /* avoid an unnecessary square when exp becomes 0 */
            base *= base;
        }
    }
    return result;
}

/* Compute A087909 for index n (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of sequence range, safe fallback */

    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            seq_t base = (seq_t)(n / d);   /* (n/d) */
            idx_t exp  = d - 1;
            sum += pow_seq(base, exp);

            /* second divisor n/d, if distinct from d */
            idx_t d2 = n / d;
            if (d2 != d) {
                base = (seq_t)d;           /* n/d2 == d */
                exp  = d2 - 1;
                sum += pow_seq(base, exp);
            }
        }
    }
    return sum;
}
