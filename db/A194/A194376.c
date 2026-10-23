/*
 * A194376  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(6) and < > denotes fractional part.
 * https://oeis.org/A194376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that Sum_{k=1..m} ( ⟨1/2
 *            + k·√6⟩ – ⟨k·√6⟩ ) = 0 (⟨x⟩ = fractional part of x). The
 *            sequence consists entirely of even numbers; it vanishes exactly
 *            when the cumulative sum of ±½ terms becomes zero. The sign of
 *            the k‑th term is +½ if ⟨k·√6⟩ < ½ and –½ otherwis...
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    idx_t count = 0;               /* how many terms found so far */
    idx_t k = 0;                   /* current index in the sequence of k */
    long long diff = 0;            /* cumulative sum of +1 / -1 signs */
    seq_t term = 0;                /* value of the current term */

    while (count < n) {
        ++k;
        seq_t k_seq = (seq_t)k;
        seq_t k_sq  = k_seq * k_seq;          /* k² */

        /* left = 24·k² */
        seq_t left = (seq_t)24 * k_sq;

        /* a = ⌊√(6·k²)⌋ */
        seq_t a_val = floor_sqrt((seq_t)6 * k_sq);

        /* right = 4·a² + 4·a + 1 */
        seq_t right = (seq_t)4 * a_val * a_val
                    + (seq_t)4 * a_val
                    + 1;

        /* sign of the k‑th term: +1 if left < right, else –1 */
        int sgn = (left < right) ? 1 : -1;
        diff += sgn;

        /* record the term if k is even and the cumulative sum is zero */
        if ( (k & 1ULL) == 0 && diff == 0 ) {
            ++count;
            term = k_seq;
        }
    }
    return term;
}
