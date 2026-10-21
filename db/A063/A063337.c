/*
 * A063337  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          64 ).
 * https://oeis.org/A063337
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 72*n - 155 for n > 2; a(2) = -1
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2) {
        return (seq_t)-1;
    }
    /* For all n > 2 the conjectured linear formula holds
       and matches the recurrence a(n)=2*a(n-1)-a(n-2). */
    return (seq_t)n * (seq_t)72 - (seq_t)155;
}
