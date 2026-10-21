/*
 * A092223  Numbers k such that numerator of Bernoulli(2*k) is divisible by
 *          101, the fourth irregular prime.
 * https://oeis.org/A092223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1934, u32 n <= 3800, u64 n <= 3800,
 *            u128 n <= 3800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A092223: Numbers k such that the numerator of Bernoulli(2*k)
 * is divisible by 101.
 *
 * Observation: The set of such k is exactly the union of
 *   • all k congruent to 34 modulo 50  (k % 50 == 34)
 *   • all k that are multiples of 101 but not multiples of 50
 *     (k % 101 == 0 && k % 50 != 0).
 * This yields all terms 34, 84, 101, 134, 184, 202, ...
 *
 * Formula: a(n) is the n-th integer k satisfying
 *           (k % 50 == 34) or (k % 101 == 0 && k % 50 != 0).
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        if ((k % 50 == 34) || (k % 101 == 0 && k % 50 != 0))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
