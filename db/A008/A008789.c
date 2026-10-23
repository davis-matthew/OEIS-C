/*
 * A008789  a(n) = n^(n+3).
 * https://oeis.org/A008789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast exponentiation by squaring.
       Since the exponent is n+3, which is at most a few thousand
       for the intended test range, this is O(log n) multiplications. */
    if (n == 0) {
        return 0;               /* 0^(3) = 0 */
    }

    seq_t result = 1;
    seq_t base   = (seq_t)n;
    idx_t exp    = n + 3;

    while (exp != 0) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp) {
            base = base * base;
        }
    }
    return result;
}
