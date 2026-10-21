/*
 * A087262  Integer quotient of largest and initial values in 3x+1 iteration,
 *          started at n.
 * https://oeis.org/A087262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A025586(n)/n) where A025586(n) is the largest value
 *            reached by the Collatz sequence starting at n. The function
 *            iterates the Collatz rules until 1 is reached, keeps the maximum
 *            value seen, and returns that maximum divided by the initial
 *            value n. The division is integer truncation (fl...
 * Range:     u8 n <= 26, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;        /* current Collatz value */
    seq_t peak = x;            /* maximum value seen so far */

    while (x > (seq_t)1) {
        /* even case: shift right */
        if ((x & (seq_t)1) == (seq_t)0) {
            x >>= 1;
        } else {                /* odd case: 3x + 1 */
            x = x * (seq_t)3 + (seq_t)1;
        }
        if (x > peak) {
            peak = x;
        }
    }
    return peak / (seq_t)n;    /* floor division */
}
