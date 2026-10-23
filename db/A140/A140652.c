/*
 * A140652  Partial sums of A062968.
 * https://oeis.org/A140652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} A062968(i) = Sum_{i=1..n} i - floor(n/(i+1))
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n*(n+1)/2 - Sum_{k=2..n} floor(n/k) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n==0 (outside offset) harmlessly */
    if (n == 0)
        return 0;

    /* Triangle number n*(n+1)/2 in seq_t */
    seq_t sumTerms = (seq_t)n * ((seq_t)n + 1ULL) / 2;

    /* Sum of floor(n/k) for k=2..n using O(sqrt(n)) block summation */
    seq_t sumFloor = 0;
    idx_t l = 2;
    while (l <= n)
    {
        idx_t q = n / l;          /* floor(n/l) */
        idx_t r = n / q;          /* largest k with floor(n/k)=q */
        if (r > n)
            r = n;
        sumFloor += (seq_t)q * ((seq_t)(r - l + 1));
        l = r + 1;
    }

    return sumTerms - sumFloor;
}
