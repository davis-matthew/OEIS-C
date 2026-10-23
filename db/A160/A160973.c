/*
 * A160973  a(n) is the number of positive integers of the form (n-3k)/(2k+1),
 *          1 <= k <= (n-1)/5.
 * https://oeis.org/A160973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 1 ≤ k ≤ (n-1)/5 and (n-3k)/(2k+1) is a positive
 *            integer }
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast return for n = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* upper bound for k : floor((n-1)/5) */
    idx_t kmax = (n - 1) / 5;

    seq_t count = (seq_t)0;
    seq_t sn = (seq_t)n;

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t num = sn - (seq_t)3 * k;          /* n - 3k     */
        seq_t den = (seq_t)2 * k + (seq_t)1;    /* 2k + 1     */

        /* Since k ≤ (n-1)/5, we have 5k ≤ n-1 ⇒ 3k ≤ n-3 ⇒ num ≥ 1.
           Nonetheless, the check below is harmless and keeps the code safe
           if the bounds ever change. */
        if (num == 0)
            continue;

        if (num % den == 0) {
            /* The quotient is automatically positive because num > 0 */
            ++count;
        }
    }

    return count;
}
