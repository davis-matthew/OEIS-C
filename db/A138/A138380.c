/*
 * A138380  First differences of A138377.
 * https://oeis.org/A138380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4 * a(n-4) for n > 3 with a(0) = 1, a(1) = 2, a(2) = -1,
 *            a(3) = -2. The general term is a(4k+r) = a(r) * (-4)^k for r =
 *            0..3
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 62, i64 n <= 126,
 *            i128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial four terms */
    static const seq_t init[4] = {(seq_t)1, (seq_t)2, (seq_t)-1, (seq_t)-2};

    /* residue modulo 4 and exponent k = floor(n/4) */
    idx_t r   = n & 3ULL;          /* n % 4 */
    idx_t exp = n >> 2ULL;        /* n / 4 */

    /* fast exponentiation of (-4)^exp in seq_t */
    seq_t pow = (seq_t)1;
    seq_t mult = (seq_t)-4;
    while (exp)
    {
        if (exp & 1ULL) pow *= mult;
        mult *= mult;
        exp >>= 1ULL;
    }

    return init[r] * pow;
}
