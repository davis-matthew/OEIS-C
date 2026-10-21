/*
 * A064753  a(n) = n*7^n - 1.
 * https://oeis.org/A064753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*7^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 7^n by binary exponentiation */
    seq_t base = (seq_t)7;
    seq_t power = (seq_t)1;
    idx_t exp = n;

    while (exp) {
        if (exp & (idx_t)1) power *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t result = (seq_t)n * power;
    return result - (seq_t)1;
}
