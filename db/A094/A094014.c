/*
 * A094014  Expansion of (1-2*x)/(1-8*x^2).
 * https://oeis.org/A094014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 8 * a(n-2), with initial values a(0) = 1, a(1) = -2. This
 *            follows from the generating function (1‑2x)/(1‑8x²)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 21, i64 n <= 41, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -2;

    seq_t result;
    if ((n & 1) == 0) {          /* even n: a(2k) = 8^k */
        result = 1;
        for (idx_t i = 0; i < n / 2; ++i)
            result *= 8;
    } else {                      /* odd n: a(2k+1) = -2 * 8^k */
        result = -2;
        for (idx_t i = 0; i < (n - 1) / 2; ++i)
            result *= 8;
    }
    return result;
}
