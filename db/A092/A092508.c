/*
 * A092508  Expansion of g.f. (1+x^18)/((1-x)*(1-x^4)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A092508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 12 n^2 + 255 n + 1152)/1152 * - (n mod 2) ·
 *            n^2/384 * + (11 · ((n+2) mod 4) - 25 · (n mod 4)) · n/768)
 * Range:     u8 none, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once in order to use
     * the arithmetic operators of seq_t. */
    seq_t sn = (seq_t)n;

    /* ---- term A : 2*(n^3 + 12 n^2 + 255 n + 1152) ---- */
    seq_t termA = sn * sn * sn;                 /* n³   */
    termA += (seq_t)12 * sn * sn;               /* +12 n² */
    termA += (seq_t)255 * sn;                   /* +255 n  */
    termA += (seq_t)1152;                      /* +1152   */
    termA *= (seq_t)2;                         /* ×2 for common denom 2304 */

    /* ---- term B : -(n mod 2)·n²/384  →  -6·(n mod 2)·n²  (denom 2304) ---- */
    seq_t termB = 0;
    if (n & 1U) {                               /* if n is odd */
        termB = (seq_t)6 * sn * sn;              /* 6·n²   */
    }

    /* ---- term C : +(11*((n+2) mod 4) - 25*(n mod 4))·n/768
     *           →  3·(11*((n+2) mod 4) - 25*(n mod 4))·n  (denom 2304) ---- */
    const idx_t mod4     = n & 3U;              /* n mod 4          */
    const idx_t mod4plus2 = (n + 2U) & 3U;      /* (n+2) mod 4      */
    const int32_t coeff = 11 * (int32_t)mod4plus2 - 25 * (int32_t)mod4; /* may be negative */

    /* Compute the absolute value of the coefficient and multiply. */
    uint64_t coeff_abs = (coeff >= 0 ? (uint64_t)coeff
                                      : (uint64_t)(-coeff));
    seq_t termC = (seq_t)coeff_abs * sn;        /* |coeff|·n   */
    termC *= (seq_t)3;                          /* ×3 for denom 2304 */

    /* ---- Combine the numerators ---- */
    seq_t num = termA;
    num -= termB;                               /* subtract B  */
    if (coeff >= 0)
        num += termC;                           /* add C       */
    else
        num -= termC;                           /* subtract |C*/

    /* ---- Final division by 2304 (floor due to unsigned division) ---- */
    return num / (seq_t)2304;
}
