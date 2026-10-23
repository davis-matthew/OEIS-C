/*
 * A155519  a(n) = Sum (J(p): p is a permutation of {1,2,...,n}), where J(p)
 *          is the number of j <= ceiling(n/2) such that p(j) + p(n+1-j) =
 *          n+1.
 * https://oeis.org/A155519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1) = n*(2n-2)!, a(2n) = 2*n^2*(2n-2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1.  For n == 0 the value is irrelevant. */
    if (n == 0) return 0;

    /* k is n/2 for even n, (n+1)/2 for odd n. */
    idx_t k = (n % 2 == 0) ? n / 2 : (n + 1) / 2;

    /* factorial of (n-2) for even n, (n-1) for odd n. */
    seq_t fact = (n % 2 == 0) ? factorial(n - 2) : factorial(n - 1);

    seq_t res;
    if (n % 2 == 0) {
        seq_t kk = (seq_t)k * (seq_t)k;   /* k^2 */
        res = fact * 2;                   /* 2 * (2n-2)! */
        res = res * kk;                   /* * k^2 = * n^2 */
    } else {
        res = fact * (seq_t)k;            /* k * (2n-2)! */
    }
    return res;
}
