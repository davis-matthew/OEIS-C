/*
 * A037478  Number of positive solutions to "numbers that are n times sum of
 *          their digits".
 * https://oeis.org/A037478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 4675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: for a given n count all positive integers x such that
 * x = n * sum_of_digits(x).  This is equivalent to counting all
 * s ≥ 1 for which digit_sum(n * s) == s.  The bound for the
 * search is chosen to be safe for n up to 10 000: s ≤ 9 * (2*digits(n)+1). */
#include "../utils/digits.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    int dn = digits(n);                  /* number of decimal digits of n */
    idx_t max_s = (idx_t)9 * (2 * dn + 1);/* safe upper bound for s   */
    seq_t count = 0;

    for (idx_t s = 1; s <= max_s; ++s) {
        seq_t x = (seq_t)n * (seq_t)s;   /* candidate number   */
        if (digit_sum(x) == (seq_t)s)    /* check digit sum    */
            ++count;                     /* valid solution     */
    }
    return count;
}
