/*
 * A109893  a(n) = least integer of the form (n!-1)(n!-2)...(n!-k)/n!.
 * https://oeis.org/A109893
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n! - 1, n) where C(N, k) is the binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For safety just return 0 for n < 3, although the sequence is only
       defined from n = 3 upwards. */
    if (n < 3) return (seq_t)0;

    /* Compute n! as a seq_t value. */
    seq_t fac = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fac *= (seq_t)i;

    /* N = n! - 1 */
    seq_t N = fac - (seq_t)1;

    /* Compute binomial coefficient C(N, n) multiplicatively:
       result = 1
       for i = 1 .. n:
           result *= (N - (n - i));
           result /= i;
       This works because the division is exact at each step. */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = N - (seq_t)(n - i);   /* N - (n - i) = N - (n+1-i-1) */
        result *= term;
        result /= (seq_t)i;
    }

    return result;
}
