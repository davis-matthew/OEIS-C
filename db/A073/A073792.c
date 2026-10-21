/*
 * A073792  Replace 5^k with (-5)^k in base 5 expansion of n.
 * https://oeis.org/A073792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(5*k+m) = -5*a(k)+m
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) where a(0)=0 and the recurrence
   a(5*k+m) = -5*a(k) + m for 0<=m<5. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t k = n / 5;
    seq_t m = (seq_t)(n % 5);
    return -(seq_t)5 * a(k) + m;
}
