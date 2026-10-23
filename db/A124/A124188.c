/*
 * A124188  Number of 3-good permutations on {1,2,...,n}, i.e., permutations
 *          that contain each of the six patterns {123, 132, 213, 231, 312,
 *          321} as a subsequence.
 * https://oeis.org/A124188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 6*C(2*n,n)/(n+1) + 5*2^n + 4*C(n,2) - 14*n -
 *            2*A000045(n+1) + 20, for n>4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, factorial, fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions in the utils directory. */
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        return (seq_t)0;
    }

    /* Convert n to seq_t for arithmetic involving seq_t */
    seq_t n_seq = (seq_t)n;

    /* Term 1: n! */
    seq_t term1 = factorial(n);

    /* Term 2: 6 * C(2*n, n) / (n+1) */
    idx_t two_n = n * (idx_t)2;
    seq_t combo = binomial(two_n, n);
    seq_t term2 = combo * (seq_t)6;
    term2 = term2 / (n_seq + (seq_t)1);

    /* Term 3: 5 * 2^n */
    seq_t term3 = pow2(n) * (seq_t)5;

    /* Term 4: 4 * C(n, 2) */
    /* For n>=5, binomial(n,2) works correctly */
    seq_t term4 = binomial(n, (idx_t)2) * (seq_t)4;

    /* Term 5: -14*n */
    seq_t term5 = (seq_t)14 * n_seq;

    /* Term 6: -2 * A000045(n+1) */
    seq_t fib_val = fib(n + (idx_t)1);
    seq_t term6 = fib_val * (seq_t)2;

    /* Combine all terms as per the formula */
    seq_t result = term1 - term2 + term3 + term4 - term5 - term6 + (seq_t)20;

    return result;
}
