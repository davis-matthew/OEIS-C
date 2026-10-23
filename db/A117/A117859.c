/*
 * A117859  Number of palindromes of length n (in base 7).
 * https://oeis.org/A117859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*7^floor((n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 (not required by the specification, but harmless) */
    if (n < 1)
        return (seq_t)0;

    idx_t exp = (n - 1) / 2;    /* floor((n-1)/2) */
    seq_t base = (seq_t)7;     /* base 7 in the required type */
    seq_t res  = (seq_t)1;     /* accumulator for 7^exp */

    while (exp > 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)6 * res;     /* multiply by 6 */
}
