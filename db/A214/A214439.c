/*
 * A214439  Denominators of correlation kernels arising in adding a list of
 *          numbers in base 3 considering the distribution of number of
 *          carries.
 * https://oeis.org/A214439
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1 if n mod 6 = 4 * a(n) = 3^e toward other n, * where e =
 *            1 + 3*⌊(n+1)/6⌋ + add(n). * add(n) = 0 for n mod 6 = 0 or 5 * =
 *            1 for n mod 6 = 1 or 2 * = 2 for n mod 6 = 3 * * For n < -1 the
 *            sequence is not defined – we return 1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < -1) {
        return (seq_t)1;
    }

    /* positive remainder modulo 6 */
    idx_t r = (n % 6 + 6) % 6;
    if (r == 4) {
        return (seq_t)1;
    }

    /* floor((n+1)/6) for n >= -1 is safe */
    idx_t b   = (n + 1) / 6;
    idx_t add = 0;
    if (r == 1 || r == 2) {
        add = 1;
    } else if (r == 3) {
        add = 2;
    }

    /* exponent e = 1 + 3*b + add */
    idx_t exp = 1 + 3 * b + add;

    /* compute 3^exp by exponentiation by squaring */
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {
            base *= base;
        }
    }
    return result;
}
