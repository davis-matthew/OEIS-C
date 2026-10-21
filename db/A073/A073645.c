/*
 * A073645  a(1)=2 and, for all n>=1, a(n) is the length of the n-th run of
 *          increasing consecutive integers with each run after the first
 *          starting with 1.
 * https://oeis.org/A073645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n=P(k)=1 + k/3 + k^2/2 + k^3/6 for some k, a(n) = 2
 *            if n is not such P(k) and P(k)-n is a triangular number,
 *            otherwise a(n)=1; a(1)=2
 * Range:     u8 n <= 11, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute the n-th term of OEIS A073645.
   n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;          /* the exceptional initial term */
    }

    /* Find smallest k such that P(k) >= n.
       P(k) = (k^3 + 3k^2 + 2k + 6) / 6 */
    idx_t k = 0;
    seq_t pk = 0;
    seq_t sn = (seq_t)n;

    do {
        ++k;
        seq_t kk = (seq_t)k;
        seq_t num = kk * kk * kk + 3 * kk * kk + 2 * kk + 6;
        pk = num / 6;
    } while (pk < sn);

    /* Now pk == P(k)  or  pk > n */
    if (pk == sn) {
        return (seq_t)3;          /* n is exactly a P(k) */
    }

    /* Check if P(k)-n is a triangular number */
    seq_t diff = pk - sn;
    seq_t d = diff * 8 + 1;       /* discriminant of m(m+1)/2 = diff */
    seq_t sd = floor_sqrt(d);
    if (sd * sd == d) {
        return (seq_t)2;
    }

    return (seq_t)1;
}
