/*
 * A036836  Schoenheim bound L_1(n,9,8).
 * https://oeis.org/A036836
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⎡ n/9 * ⎡ (n-1)/8 * ⎡ (n-2)/7 * ⎡ (n-3)/6 * ⎡ (n-4)/5 * ⎡
 *            (n-5)/4 * ⎡ (n-6)/3 * ⎡ (n-7)/2 ⎤ ⎤ ⎤ ⎤ ⎤ ⎤ (Schoenheim bound
 *            L1(n,9,8))
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (unused value). */
    if (n < 9)
        return 0;

    seq_t cur = 1;                     /* innermost value */
    for (idx_t offset = 0; offset < 8; ++offset) {
        /* Numerator: n-7+offset   (n-7, n-6, …, n) */
        seq_t num = (seq_t)(n - 7 + offset);
        /* Denominator: 2+offset   (2,3,…,9) */
        seq_t den = (seq_t)(2 + offset);

        /* Compute ceil(num * cur / den) without overflow:
           ceil(a*b/c) = floor(a/c)*b + ceil((a%c)*b/c)   (c > 0) */
        seq_t q   = cur / den;          /* cur / den  */
        seq_t r   = cur % den;          /* cur % den  */
        seq_t part = q * num;           /* floor part */
        seq_t rem  = r * num;           /* remainder part */
        seq_t rem_c = (rem + den - 1) / den; /* ceil of remainder part */
        cur = part + rem_c;
    }
    return cur;
}
