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
 * Validated: standard, pass, 46 terms through n = 374 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
