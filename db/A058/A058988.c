/*
 * A058988  For a rational number p/q let f(p/q) = p*q divided by number of
 *          divisors of p+q; a(n) is obtained by iterating f, starting at n/1,
 *          until an integer is reached, or if no integer is ever reached then
 *          a(n) = 0.
 * https://oeis.org/A058988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   iterate f(p/q) = p*q / d(p+q) starting from n/1 until an integer
 *            is reached. * d(k) is the number of divisors of k. Return that
 *            integer, or 0 if the process never * reaches an integer.
 *            Sequence A058988
 * Range:     u8 n <= 34, u16 n <= 1762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Handle the theoretical offset; a(0) is undefined but return 0. */
    if (n == 0) return 0;

    seq_t p = (seq_t)n;    /* numerator */
    seq_t q = 1;           /* denominator */

    /* Safety counter to avoid an infinite loop in the unlikely event of a cycle. */
    const size_t MAX_STEPS = 100000;
    size_t step = 0;

    while (step < MAX_STEPS) {
        ++step;
        seq_t prod = p * q;                    /* product of numerator and denominator */
        uint64_t sum = (uint64_t)(p + q);      /* p+q must fit in 64-bit for divisor count */
        uint64_t d = divisor_count_u64(sum);   /* number of divisors of p+q */

        /* If the product is divisible by d, an integer is reached. */
        if (prod % (seq_t)d == 0) {
            return prod / (seq_t)d;
        }

        /* Else set the new rational to prod/d and reduce it. */
        p = prod;
        q = (seq_t)d;

        seq_t g = gcd(p, q);
        if (g > 1) {
            p = p / g;
            q = q / g;
        }

        /* In the very unlikely case q becomes 1 but prod not divisible by d,
         * we treat it as a terminal non‑integer and return 0. */
        if (q == 1) {
            return 0;
        }
    }

    /* If the process did not terminate within MAX_STEPS, assume it never
     * reaches an integer. */
    return 0;
}
