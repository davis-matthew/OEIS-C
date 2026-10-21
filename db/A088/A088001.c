/*
 * A088001  a(n) is the sum of non-palindromic divisors of n.
 * https://oeis.org/A088001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of non‑palindromic divisors of n * (OEIS sequence
 *            A088001)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute all divisors d of n (1 ≤ d ≤ n). For each divisor,
 * if d is not a decimal palindrome, we add it to the sum.  Since
 * n fits in idx_t (uint64_t), all divisors also fit in uint64_t.
 * The sum is accumulated in a variable of type seq_t to avoid
 * overflow in the intermediate steps.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_uint64.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 */
    uint64_t un = (uint64_t)n;          /* use native integer for divisor loop */
    seq_t sum = 0;                       /* accumulate result in seq_t */

    /* iterate all divisor pairs (d, n/d) */
    for (uint64_t d = 1; d <= un / d; ++d) {
        if (un % d == 0) {
            uint64_t q = un / d;          /* paired divisor */

            /* handle divisor d */
            if (!is_palindrome_uint64(d))
                sum += (seq_t)d;

            /* handle paired divisor q if distinct */
            if (q != d && !is_palindrome_uint64(q))
                sum += (seq_t)q;
        }
    }

    return sum;
}
