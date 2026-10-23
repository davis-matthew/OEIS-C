/*
 * A120740  Numbers n such that n = Sum_digits[k*abs(n-k)] for some k>=0.
 * https://oeis.org/A120740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (18*n - (-1)^n - 35)/4 for n > 2
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms: a(1) = 0, a(2) = 4 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* just in case */
    if (n == 1) return 0;
    if (n == 2) return 4;

    seq_t nn = (seq_t)n;
    seq_t numer;

    if (n & 1)                      /* odd n */
        numer = nn * 18 - 34;       /* 18*n - 34 */
    else                            /* even n */
        numer = nn * 18 - 36;       /* 18*n - 36 */

    return numer / 4;               /* division is exact */
}
