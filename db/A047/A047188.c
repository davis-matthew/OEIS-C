/*
 * A047188  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(n/5, ⌊(n-2)/2⌋/2, ⌊(n-⌊(n-2)/2⌋⌋/3)} [
 *            C(⌊(n-2)/2⌋, 2k) * C(n‑⌊(n-2)/2⌋, 3k) ] where C(n,k) is the
 *            binomial coefficient
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a direct combinatorial count of non‑empty subsets of {1,…,n}
   where exactly 2/5 of the chosen elements lie in the first floor((n‑2)/2) integers.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* left side size */
    idx_t l = (n <= 2) ? (idx_t)0 : (n - 2) / 2;
    /* right side size */
    idx_t h = n - l;

    /* maximum k given the size constraints */
    idx_t kmax = n / 5;
    if (l / 2 < kmax) kmax = l / 2;
    if (h / 3 < kmax) kmax = h / 3;

    seq_t result = 0;

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t low  = binomial(l,           2 * k);
        seq_t high = binomial(h,           3 * k);
        result += low * high;
    }

    return result;
}
