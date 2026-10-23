/*
 * A182660  a(2^(k+1)) = k; 0 everywhere else.
 * https://oeis.org/A182660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for all n that are not powers of two; a(2^(k+1)) = k
 *            (so a(0)=0, a(1)=0, a(2)=0, a(4)=1, a(8)=2, …)
 * Range:     u8 n <= 8524, u16 n <= 8524, u32 n <= 8524, u64 n <= 8524,
 *            u128 n <= 8524, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for index 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Check if n is a power of two */
    if ((n & (n - 1)) == 0) {
        /* Compute exponent m such that n = 2^m */
        idx_t m = 0;
        idx_t tmp = n;
        while (tmp > 1) {
            tmp >>= 1;
            ++m;
        }
        /* For n = 1 (m = 0) the sequence defines a(1) = 0.
           For m >= 1 we return k = m-1. */
        if (m >= 1) {
            return (seq_t)(m - 1);
        }
    }

    /* n is not a power of two (or is 1) */
    return (seq_t)0;
}
