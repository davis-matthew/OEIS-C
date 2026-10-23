/*
 * A176043  a(n) = (2*n-1)*(n-1)^(n-1).
 * https://oeis.org/A176043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(n-1)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases where (n-1)^(n-1) is defined as 1 */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    /* Compute (n-1)^(n-1) using exponentiation by squaring */
    seq_t base = (seq_t)(n - 1);
    seq_t result = (seq_t)1;
    seq_t power  = base;
    idx_t exp = n - 1;

    while (exp > 0) {
        if (exp & 1ULL) {
            result = result * power;
        }
        exp >>= 1;
        if (exp) {
            power = power * power;
        }
    }

    /* Multiply by (2*n-1) */
    seq_t mult = (seq_t)(2 * n - 1);
    return mult * result;
}
