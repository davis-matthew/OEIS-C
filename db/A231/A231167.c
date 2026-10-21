/*
 * A231167  a(1) = a(2) = 0, for n>=3: (sum of non-divisors of n) modulo
 *          (number of non-divisors of n).
 * https://oeis.org/A231167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2 - sigma(n)) mod (n - d(n)) (for n >= 3);
 *            a(1)=a(2)=0
 * Range:     u8 n <= 24, u16 n <= 938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* may be provided by the build environment */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2)
        return 0;

    /* Sum of all numbers from 1 to n */
    seq_t n_seq = (seq_t)n;
    seq_t sum_all = n_seq * (n_seq + 1) / 2;

    /* Compute the number of divisors d(n) and the sum of divisors sigma(n) */
    idx_t count_div = 0;        /* d(n) */
    seq_t sum_div   = 0;        /* sigma(n) */

    /* Iterate over potential divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            /* divisor pair i, j (could be equal) */
            count_div += (i == j) ? 1 : 2;
            sum_div += (seq_t)i;
            if (i != j)
                sum_div += (seq_t)j;
        }
    }

    /* Number of non‑divisors and the sum of non‑divisors */
    idx_t noncnt = n - count_div;          /* n - d(n)  */
    /* For n=1,2 this is zero; we already handled those cases */
    seq_t sum_nondiv = sum_all - sum_div;  /* sum_all - sigma(n) */

    /* Return the result modulo the number of non‑divisors */
    return sum_nondiv % (seq_t)noncnt;
}
