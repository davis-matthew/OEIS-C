/*
 * A158552  a(n) = A144433(n) - A106833(n).
 * https://oeis.org/A158552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-2) + a(n-3), with a(1)=5, a(2)=-1, a(3)=7
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A158552. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1: undefined for n==0 */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)7;

    seq_t a1 = (seq_t)5;   /* a(n-3) */
    seq_t a2 = (seq_t)-1;  /* a(n-2) */
    seq_t a3 = (seq_t)7;   /* a(n-1) */
    seq_t curr;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        curr = -a3 + a2 + a1;   /* a(n) = -a(n-1) + a(n-2) + a(n-3) */
        a1 = a2;                /* shift window: a(n-3) = a(n-2) */
        a2 = a3;                /* a(n-2) = a(n-1) */
        a3 = curr;              /* a(n-1) = a(n) */
    }

    return a3;
}
