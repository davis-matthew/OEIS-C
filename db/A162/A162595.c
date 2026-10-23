/*
 * A162595  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) * (1-x^18) * (1-x^21) / (1-x)^7.
 * https://oeis.org/A162595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162595:
   a(n) = sum_{S⊆{3,6,9,12,15,18,21}} (-1)^|S| * binomial(n - sum(S) + 6, 6)
   where the term is taken only if n ≥ sum(S).  This implements the
   generating function (1‑x³)(1‑x⁶)…(1‑x²¹)/(1‑x)⁷.
   The implementation uses inclusion–exclusion with 7 shifts, precomputing
   all subset sums and signs once.  It works for any seq_t/idx_t types
   allowed by the build.  Complexity is O(2⁷) per call (constant time).  */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Pre‑compute all subset sums of {3,6,9,12,15,18,21} and signs. */
    static idx_t  subsets[128];
    static int    signs[128];
    static int    ready = 0;

    if (!ready) {
        const idx_t shifts[7] = {3, 6, 9, 12, 15, 18, 21};
        for (idx_t m = 0; m < 128; ++m) {
            idx_t  s = 0;
            int    pc = 0;
            for (int i = 0; i < 7; ++i) {
                if (m & (1ULL << i)) {
                    s += shifts[i];
                    ++pc;
                }
            }
            subsets[m] = s;
            signs[m]   = (pc & 1) ? -1 : 1;   /* even → +1, odd → –1 */
        }
        ready = 1;
    }

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t m = 0; m < 128; ++m) {
        idx_t s = subsets[m];
        if (n < s) continue;                     /* term vanishes */

        idx_t arg = n - s + 6;                   /* n - sum(S) + 6 */
        seq_t term = binomial(arg, 6);            /* binomial(arg,6) */

        if (signs[m] == 1)
            pos += term;
        else
            neg += term;
    }

    return pos - neg;  /* pos ≥ neg by construction */
}
