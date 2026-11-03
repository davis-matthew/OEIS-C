/*
 * A076511  Numerator of cototient(n)/totient(n).
 * https://oeis.org/A076511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n - φ(n)) / gcd(n - φ(n), φ(n)) (the numerator of the
 *            reduced fraction (n-φ(n))/φ(n) )
 * Range:     u8 n <= 375, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 8284 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* safety; the sequence is defined for n ≥ 1 */
        return 0;

    seq_t phival = totient(n);                 /* φ(n) */
    seq_t cot = (seq_t)n - phival;         /* n - φ(n)      */
    seq_t g = gcd((seq_t)n, phival);       /* gcd(n, φ(n))  */
    return cot / g;                        /* integer division, exact */
}
