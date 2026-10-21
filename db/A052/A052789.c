/*
 * A052789  Expansion of e.g.f. -(1/5)*LambertW(-5*x).
 * https://oeis.org/A052789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n)^(n-1) for n > 0 , a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* base = 5 * n  */
    seq_t base = (seq_t)n * 5;

    /* exponent = n-1  */
    idx_t exp = n - 1;

    /* fast exponentiation:  base^exp */
    seq_t result = 1;
    while (exp > 0) {
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
