/*
 * A088346  Smallest integer k for which exp(x) > x^n, for all x>=k, n>=3.
 * https://oeis.org/A088346
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer k > n such that k > n * ln(k)
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the smallest k with e^k > k^n for all x >= k, n >= 3 */

#include <math.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3.
       For smaller n we return 0 (value is irrelevant). */
    if (n < 3) return (seq_t)0;

    /* Start looking for k from n+1, because for x > n the function
       h(x) = x - n*ln(x) is strictly increasing.  The first
       integer k > n such that k > n*ln(k) will be our answer. */
    for (idx_t k = n + 1; ; ++k)
    {
        double dk = (double)k;
        double nk = (double)n * log(dk);

        if (dk > nk)
            return (seq_t)k;
    }
}
