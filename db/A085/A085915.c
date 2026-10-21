/*
 * A085915  Group the natural numbers such that the product of the terms of
 *          the n-th group is divisible by n!: (1), (2), (3, 4), (5, 6, 7, 8),
 *          (9, 10, 11, 12), (13, 14, 15, 16, 17, 18), (19, 20, 21, 22, 23,
 *          24), ... Sequence contains the product of the terms of the n-th
 *          group divided by n!. a(n) = A085912(n)...
 * https://oeis.org/A085915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   group_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085915 – OEIS sequence “OEIS sequence A085915”.
 *
 *  Let g(n) be the size of the n‑th group of natural numbers:
 *        g(1) = g(2) = 1
 *        g(n) = n      if n is even (n ≥ 3)
 *        g(n) = n‑1    if n is odd  (n ≥ 3)
 *
 *  The first term of group n is
 *        start(n) = 1 + Σ_{k=1}^{n-1} g(k)
 *
 *  The value of the sequence is
 *        a(1) = a(2) = 1
 *
 *        n even :  a(n) = C(start(n)+n‑1 , n)
 *        n odd  :  a(n) = C(start(n)+n‑2 , n‑1) / n
 *
 *  The binomial coefficient is evaluated multiplicatively; division
 *  is exact.  All calculations use the type seq_t, which may be
 *  8, 16, 32, 64 or 128‑bit, or an arbitrary‑precision integer
 *  class.  This implementation requires only the standard C
 *  operators and functions; it is O(n) in the size of the largest
 *  n requested, which is well below the 2‑second limit for n ≤ 10000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/group_size.h"

/* Multiplicative binomial coefficient C(N,K) returning seq_t. */
static seq_t binomial(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0) return (seq_t)1;
    if (K > N - K) K = N - K;   /* use symmetry */

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        /* res = res * (N-K+i) / i;  exact division */
        res = res * (seq_t)(N - K + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (n == 1 || n == 2) return (seq_t)1;

    /* Compute start(n) = 1 + Σ_{k=1}^{n-1} g(k) */
    idx_t start = 1;
    for (idx_t k = 1; k < n; ++k) {
        start += group_size(k);
    }

    if ((n & 1) == 0) {                 /* n is even */
        idx_t N = start + n - 1;
        return binomial(N, n);
    } else {                             /* n is odd, n > 2 */
        idx_t m = n - 1;
        idx_t N = start + m - 1;         /* start + n - 2 */
        seq_t val = binomial(N, m);
        return val / (seq_t)n;           /* exact division */
    }
}
