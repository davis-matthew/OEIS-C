/*
 * A151577  Number of permutations of 1..n arranged in a circle with exactly 4
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151577
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(4^(n-1) - n*3^(n-1) + binomial(n,2)*2^(n-1) -
 *            binomial(n,3))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/binomial.h"

/* Return the nth term of OEIS A151577 for n ≥ 4.
   Terms for n < 4 are undefined; this implementation simply returns 0. */
seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;          /* behaviour below the offset is irrelevant */

    idx_t m = n - 1;                     /* exponent for the powers */

    /* Compute each part of the formula. */
    seq_t part1 = pow4(m);                         /* 4^(n-1) */
    seq_t part2 = (seq_t)n * pow3(m);             /* n * 3^(n-1) */
    seq_t part3 = binomial(n, 2) * pow2(m);      /* C(n,2) * 2^(n-1) */
    seq_t part4 = binomial(n, 3);                /* C(n,3) */

    seq_t inner = part1 - part2 + part3 - part4;  /* the parentheses */

    return (seq_t)n * inner;                      /* final multiplication by n */
}
