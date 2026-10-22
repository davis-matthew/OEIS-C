/*
 * A006184  Number of cycles in the complement of a path.
 * https://oeis.org/A006184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{j=0..n-1} A000166(j) - n*(n+1)/2 + 2*(n-1))/2 for n
 *            >= 5, where A000166(j) = (j-1)*(A000166(j-1) + A000166(j-2)),
 *            A000166(0) = 1, A000166(1) = 0. This comes from the triple-sum
 *            formula by summing it with generating functions. The code keeps
 *            each D(j) as 2*h(j) + p(j) with p(j) in {0,...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 5) return 0;

    seq_t h2 = 0;          /* floor(D(0)/2) */
    seq_t h1 = 0;          /* floor(D(1)/2) */
    long long p2 = 1;      /* D(0) mod 2 */
    long long p1 = 0;      /* D(1) mod 2 */
    seq_t H = 0;           /* sum of floor(D(j)/2) */
    long long odd = 1;     /* number of odd D(j) so far */

    for (idx_t j = 2; j <= n - 1; j++) {
        long long q = (long long)(j - 1) * (p1 + p2);
        seq_t hj = (seq_t)(j - 1) * (h1 + h2);
        hj += (seq_t)(q / 2);
        long long pj = q % 2;
        H += hj;
        odd += pj;
        h2 = h1; h1 = hj;
        p2 = p1; p1 = pj;
    }

    long long nn = (long long)n;
    long long C = nn * (nn + 1) / 2 - 2 * (nn - 1);
    long long t = (odd - C) / 2;
    if (t >= 0) H += (seq_t)t;
    else H -= (seq_t)(-t);
    return H;
}
