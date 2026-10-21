/*
 * A058184  "Real rabbits": a(n) = Re(c(n)) where complex c(n) = a(n) + i*b(n)
 *          and c(0) = i, c(1) = -i, c(n) = c(n-1) + i*c(n-2).
 * https://oeis.org/A058184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=-1, a(3)=0; for n>=4: a(n) = 2*a(n-1) -
 *            a(n-2) - a(n-4) (linear recurrence)
 * Range:     i8 n <= 17, i16 n <= 32, i32 n <= 61, i64 n <= 123,
 *            i128 n <= 243, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2)          return (seq_t)(-1);
    if (n == 3)          return (seq_t)0;

    /* iterate using the recurrence, keeping last four terms */
    seq_t a_n_minus4 = (seq_t)0;   /* a(0) */
    seq_t a_n_minus3 = (seq_t)0;   /* a(1) */
    seq_t a_n_minus2 = (seq_t)(-1);/* a(2) */
    seq_t a_n_minus1 = (seq_t)0;   /* a(3) */
    seq_t a_n = 0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)(2 * a_n_minus1 - a_n_minus2 - a_n_minus4);

        /* shift window for next iteration */
        a_n_minus4 = a_n_minus3;
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = a_n;
    }
    return a_n;
}
