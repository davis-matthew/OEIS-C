/*
 * A181953  Least nonnegative k such that 2*t(n) + t(k) is prime, where t(n) =
 *          n *(n+1)/2, the n-th triangular number.
 * https://oeis.org/A181953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 0 | 2·T(n−1) + T(k) is prime}, where T(m) =
 *            m·(m+1)/2 is the m‑th triangular number
 * Range:     u8 n <= 16, u16 n <= 348, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /*  n is 1‑based (offset 1).  Translate to 0‑based index for T.  */
    idx_t n0 = n - 1;
    seq_t tn = (seq_t)n0 * ((seq_t)n0 + 1) / 2;   /* T(n−1) */

    for (idx_t k = 0;; ++k) {
        seq_t tk = (seq_t)k * ((seq_t)k + 1) / 2;   /* T(k) */
        seq_t cand = (tn << 1) + tk;                /* 2·T(n−1) + T(k) */
        if (isPrimeGeneric(cand))
            return (seq_t)k;
    }
}
