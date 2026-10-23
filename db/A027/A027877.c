/*
 * A027877  a(n) = Product_{i=1..n} (9^i - 1).
 * https://oeis.org/A027877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (9^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A027877.
 * a(0) = 1, a(1) = 8, a(2) = 640, ...
 * The computation uses only integer arithmetic and is
 * compatible with both fixed-width integer types and an
 * arbitrary-precision seq_t class (operator* and operator-).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res   = (seq_t)1;   /* running product */
    seq_t pow9  = (seq_t)9;   /* 9^i for i = 1 initially */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = pow9 - (seq_t)1;
        res = res * term;          /* multiply into running product */
        pow9 = pow9 * (seq_t)9;     /* advance to next power of 9 */
    }

    return res;
}
