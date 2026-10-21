/*
 * A085531  a(n) = (2*n+1)^(2*n-1).
 * https://oeis.org/A085531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0, as (2*0+1)^(-1) is undefined in integer arithmetic */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t base = (seq_t)(2 * n + 1);
    idx_t exp   = 2 * n - 1;
    seq_t result = (seq_t)1;

    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return result;
}
