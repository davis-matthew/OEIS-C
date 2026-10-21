/*
 * A213268  Denominators of the Inverse semi-binomial transform of A001477(n)
 *          read downwards antidiagonals.
 * https://oeis.org/A213268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A213268
    Denominators of the inverse semi‑binomial transform of A001477(n)
    read downwards antidiagonals.
    Formula: 
      let   s = ⌊(√(8n+1) − 1)/2⌋          // antidiagonal index
      let   p = n − s(s+1)/2                // position within the diagonal
      let   m = s + p                      // numerator of T(p,s−p)
      let   v = v₂(m)                       // exponent of 2 dividing m
      then   a(n) = 2^{ max(0, p − v) }      // 1 if m = 0
    (v₂ is 0 for m = 0; the 2‑adic valuation of a positive integer.)
*/

#include <stdbool.h>
#include <math.h>
#include "../utils/pow2.h"          // pow2(exp) → 2^exp as seq_t

seq_t a(idx_t n)
{
    if (n == 0)                    // special case m = 0
        return (seq_t)1;

    /* --- find antidiagonal index s such that
           s(s+1)/2 ≤ n < (s+1)(s+2)/2   */
    long double sqrt_val = sqrt((long double)(8ull * n + 1));
    idx_t s = (idx_t)((sqrt_val - 1.0L) / 2.0L);

    /* adjust in case of rounding errors */
    unsigned __int128 term = (unsigned __int128)s * (s + 1) / 2;
    while (term > n) {
        s--;
        term = (unsigned __int128)s * (s + 1) / 2;
    }
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) {
        s++;
    }

    idx_t p = n - (idx_t)(term);          // offset within diagonal
    idx_t m = s + p;                      // numerator

    if (m == 0)
        return (seq_t)1;                  // numerator 0 → integer 0

    /* v₂(m): number of trailing zeros in binary representation */
    unsigned v = __builtin_ctzll((unsigned long long)m);

    unsigned e = (p > v) ? (p - v) : 0;   // exponent of 2 for the denominator

    return pow2((idx_t)e);
}
