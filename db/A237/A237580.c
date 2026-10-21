/*
 * A237580  a(n) = (2n)! - n! + 1.
 * https://oeis.org/A237580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! - n! + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t factorial = 1;     /* will hold (2*n)! */
    seq_t factorial_n = 1;   /* will hold n!   */
    idx_t i;

    /* Compute factorial up to 2*n.
       When i == n, record n! to reuse for subtraction.  */
    for (i = 1; i <= 2 * n; ++i) {
        factorial *= (seq_t)i;
        if (i == n) {
            factorial_n = factorial;
        }
    }

    /* a(n) = (2*n)! - n! + 1 */
    return factorial - factorial_n + (seq_t)1;
}
