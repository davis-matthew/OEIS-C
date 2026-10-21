/*
 * A098769  Abundant numbers whose reversal is also an abundant number.
 * https://oeis.org/A098769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 855, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses brute‑force search to find the n‑th number that is
 * abundant and whose decimal reversal is also abundant.
 * The sum of divisors is computed via a simple divisor loop.
 * Complexity: O(n · sqrt(value)), fast enough for n up to ~10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/rev.h"
#include "../utils/int_sqrt.h"

/* Sum of positive divisors of v (including v itself). */
static seq_t sigma_of(idx_t v)
{
    seq_t sum = 1;          /* divisor 1 is always present */
    if (v <= 1) return 1;   /* sigma(1) == 1 */

    idx_t sq = int_sqrt(v);
    for (idx_t d = 2; d <= sq; ++d)
    {
        if (v % d == 0)
        {
            seq_t q = (seq_t)(v / d);
            sum += (seq_t)d;
            if (q != d) sum += q;   /* avoid double counting sqrt */
        }
    }
    sum += (seq_t)v;          /* add the number itself */
    return sum;
}

/* Checks whether v is an abundant number (σ(v) > 2v). */
static bool abundant(idx_t v)
{
    if (v == 0) return false;
    seq_t sigma = sigma_of(v);
    seq_t twice = (seq_t)v * 2;
    return sigma > twice;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    idx_t found = 0;
    for (idx_t num = 1;; ++num)
    {
        if (abundant(num) && abundant(rev(num)))
        {
            ++found;
            if (found == n) return (seq_t)num;
        }
    }
}
