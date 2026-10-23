/*
 * A123025  a(n) = n!*b(n), where b(n) = (1 + n - n^2)*b(n-2)/(n*(n-1)), b(0)
 *          = b(1) = 1.
 * https://oeis.org/A123025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1 + n - n^2) * a(n-2) with a(0) = a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A123025. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    /* Even indices use a(0) as seed, odd indices use a(1) */
    if ((n & (seq_t)1) == 0) {             /* n is even */
        seq_t cur = (seq_t)1;              /* a(0) */
        for (idx_t i = 2; i <= n; i += 2) {
            seq_t factor = (seq_t)1 + (seq_t)i - (seq_t)i * (seq_t)i;
            cur = factor * cur;
        }
        return cur;
    } else {                               /* n is odd */
        seq_t cur = (seq_t)1;              /* a(1) */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t factor = (seq_t)1 + (seq_t)i - (seq_t)i * (seq_t)i;
            cur = factor * cur;
        }
        return cur;
    }
}
