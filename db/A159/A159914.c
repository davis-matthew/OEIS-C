/*
 * A159914  Half the number of (n-3)-element subsets of {1,...,n} whose
 *          elements sum up to an odd value.
 * https://oeis.org/A159914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{k odd} C(o,k) * C(e, n-3-k), where * o =
 *            floor((n+1)/2) (number of odds in {1..n}), * e = floor(n/2)
 *            (number of evens in {1..n}), * m = n-3 (subset size). * For even
 *            n (n=2k) the sum simplifies to a(n) = k(k-1)(2k-1)/6
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"


seq_t a(idx_t n)
{
    /* For n = 0,1,2,3 the number of (n‑3)-element subsets is zero. */
    if (n < 4) return 0;

    /* Even n: use the closed‑form polynomial. */
    if ((n & 1) == 0) {
        idx_t k = n / 2;
        seq_t kk = (seq_t)k;
        seq_t res = kk * (kk - 1) * (2 * kk - 1);
        return res / 6;
    }

    /* Odd n: evaluate the alternating‑parity sum. */
    idx_t o = (n + 1) / 2;          /* count of odd numbers in {1..n}      */
    idx_t e = n / 2;                /* count of even numbers in {1..n}     */
    idx_t m = n - 3;                /* size of the subsets to choose      */

    seq_t sum = 0;
    /* k runs over odd values ≤ min(o, m) */
    for (idx_t k = 1; k <= o && k <= m; k += 2) {
        seq_t term = binomial(o, k);           /* C(o,k)  */
        term *= binomial(e, m - k);           /* C(e,m-k) */
        sum += term;
    }
    return sum / 2;
}
