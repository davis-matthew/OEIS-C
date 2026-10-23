/*
 * A014908  Inverse of 899th cyclotomic polynomial.
 * https://oeis.org/A014908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (⌊n/899⌋+1) - (n>=29? ⌊(n-29)/899⌋+1 :0) - (n>=31?
 *            ⌊(n-31)/899⌋+1 :0) + (n>=60? ⌊(n-60)/899⌋+1 :0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* term from factor (x-1)^{-1} */
    seq_t t0 = (seq_t)(n / 899 + 1);

    /* terms from factors (x^29-1) and (x^31-1) */
    seq_t t1 = 0;
    if (n >= 29) {
        t1 = (seq_t)((n - 29) / 899 + 1);
    }
    seq_t t2 = 0;
    if (n >= 31) {
        t2 = (seq_t)((n - 31) / 899 + 1);
    }

    /* term from factor (x^899-1)^{-1} */
    seq_t t3 = 0;
    if (n >= 60) {
        t3 = (seq_t)((n - 60) / 899 + 1);
    }

    return t0 - t1 - t2 + t3;
}
