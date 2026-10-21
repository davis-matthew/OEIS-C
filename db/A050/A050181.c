/*
 * A050181  T(2n+3, n), array T as in A051168; a count of Lyndon words.
 * https://oeis.org/A050181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = (binom(2n+3, n) - (3|n ? binom(2n/3+1, n/3) :
 *            0)) / (2n+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    /* denominator 2*n+3 */
    idx_t den = 2 * n + 3;

    /* main term binom(2n+3, n) */
    seq_t num = binom(den, n);

    /* correction when 3 divides n */
    if (n % 3 == 0) {
        idx_t n3 = n / 3;                 /* n/3 */
        idx_t den3 = 2 * n3 + 1;          /* (2*n/3)+1 */
        num = num - binom(den3, n3);      /* subtract binom(2n/3+1, n/3) */
    }

    /* final division; the division is exact */
    return num / (seq_t)den;
}
