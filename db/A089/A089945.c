/*
 * A089945  Main diagonal of array A089944, in which the n-th row is the n-th
 *          binomial transform of the natural numbers.
 * https://oeis.org/A089945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*(n+1)^(n-1) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* a(0) = 1 */

    seq_t base = (seq_t)(n + 1);                /* n+1 */
    idx_t exp  = n - 1;                         /* exponent n-1 */

    /* fast exponentiation: base^exp */
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1) result *= cur;            /* result = result * cur */
        cur *= cur;                            /* cur = cur * cur */
        exp >>= 1;
    }

    seq_t factor = (seq_t)(2 * n + 1);          /* 2*n+1 */
    return result * factor;                    /* a(n) = base^exp * factor */
}
