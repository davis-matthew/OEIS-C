/*
 * A175971  Denominators of 1/16-1/m^2 for some m>0, which are of the form
 *          (8*k+4)^2, sorted by increasing m.
 * https://oeis.org/A175971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175971: Denominators of 1/16-1/m^2 for m>0 that are of the form (8*k+4)^2,
   sorted by increasing m.  This function computes the n-th term by iterating
   over m, computing the reduced denominator and selecting those that satisfy
   the square‑form condition.  The algorithm is O(n * sm), where sm is the
   average m needed for the n-th term, well below the 2‑second limit for n up
   to 10 000.  */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1; behaviour irrelevant */
        return 0;

    idx_t count = 0;
    idx_t m     = 1;          /* current m to test */

    while (1) {
        seq_t m_sqr = (seq_t)m * (seq_t)m;           /* m^2          */
        seq_t denom_full = (seq_t)16 * m_sqr;        /* 16*m^2       */

        /* |m^2 - 16| */
        seq_t abs_num = (m_sqr >= (seq_t)16) ?
                         (m_sqr - (seq_t)16) : ((seq_t)16 - m_sqr);

        seq_t g = gcd(abs_num, denom_full);
        seq_t denom = denom_full / g;                 /* fully reduced */

        seq_t sqrt_d = floor_sqrt(denom);
        if (sqrt_d * sqrt_d == denom && (sqrt_d & 7ULL) == 4ULL) {
            ++count;
            if (count == n)
                return denom;                         /* a(n) found   */
        }
        ++m;                                            /* next m       */
    }
}
