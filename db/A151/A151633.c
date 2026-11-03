/*
 * A151633  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20^n - (3*n + 1)*10^n + binomial(3*n+1,2)*4^n -
 *            binomial(3*n+1,3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 159 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  For n==0 we return 0 as a guard. */
    if (n == 0)
        return 0;

    /* Powers */
    seq_t pow20 = ipow((seq_t)20, (seq_t)n);
    seq_t pow10 = ipow((seq_t)10, (seq_t)n);
    seq_t pow4  = ipow((seq_t)4,  (seq_t)n);

    /* Binomial coefficients */
    idx_t t = 3*n + 1;
    seq_t binom2 = binomial(t, 2);
    seq_t binom3 = binomial(t, 3);

    /* Intermediate terms using only seq_t arithmetic */
    seq_t term2 = seq_mul(pow10, (seq_t)t);   /* (3*n+1)*10^n */
    seq_t term3 = seq_mul(binom2, pow4);      /* binom(3*n+1,2)*4^n */

    /* rearrange to keep all intermediate results non‑negative:
       a(n) = (20^n + term3) - (term2 + binom3) */
    seq_t sum1 = pow20 + term3;
    seq_t sum2 = term2 + binom3;
    return sum1 - sum2;
}
