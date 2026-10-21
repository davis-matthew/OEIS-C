/*
 * A228231  Number of equivalence classes in S_n under the {123, 321} {132,
 *          231}-equivalence.
 * https://oeis.org/A228231
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..floor(n/2)} k!*C(n-k-1,ceil(n/2)-1) +
 *            sum_{k=1..ceil(n/2)} k!*C(n-k-1,floor(n/2)-1) where
 *            C(a,b)=binomial(a,b)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 66,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

/* Compute the a(n) value for the OEIS sequence A228231.
   Indexing follows OEIS: a(3) is the first term.  */
seq_t a(idx_t n) {
    /* The sequence is defined only for n >= 3.  For smaller n we return 0. */
    if (n < 3) return (seq_t)0;

    /* l = floor(n/2), h = ceil(n/2) */
    idx_t l = n / 2;
    idx_t h = (n + 1) / 2;

    seq_t result = 0;
    seq_t fact = 1;          /* fact = k! */

    for (idx_t k = 1; k <= h; ++k) {
        /* Update factorial: fact = k! */
        fact = mul_small(fact, (unsigned int)k);

        idx_t nk1 = n - k - 1;          /* n-k-1, always >= 0 */

        /* First sum: add term if k <= l */
        if (k <= l) {
            seq_t c1 = binomial(nk1, h - 1);
            seq_t term1 = seq_mul(fact, c1);
            result += term1;
        }

        /* Second sum: add for all k <= h (always true here) */
        seq_t c2 = binomial(nk1, l - 1);
        seq_t term2 = seq_mul(fact, c2);
        result += term2;
    }

    return result;
}
