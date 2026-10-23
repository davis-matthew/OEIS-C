/*
 * A048079  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} binomial(A,4*k)*binomial(B,k), where A =
 *            floor((n+2)/3), B = n - A, and k is chosen so that 4*k <= A and
 *            k <= B
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t m = (n + 2) / 3;         /* A = floor((n+2)/3) */
    idx_t A = m;
    idx_t B = n - m;
    idx_t kmax1 = A / 4;          /* floor(A/4) */
    idx_t kmax2 = B;              /* B itself */
    idx_t Kmax = kmax1 < kmax2 ? kmax1 : kmax2;  
    seq_t total = 0;
    for (idx_t k = 1; k <= Kmax; ++k) {
        seq_t c1 = binomial(A, 4 * k);
        seq_t c2 = binomial(B, k);
        total += c1 * c2;
    }
    return total;
}
