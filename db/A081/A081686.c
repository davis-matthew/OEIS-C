/*
 * A081686  a(n) = 7^n - 6^n - 5^n - 4^n + 3*3^n.
 * https://oeis.org/A081686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 7^n - 6^n - 5^n - 4^n + 3*3^n Recurrence: a(n) =
 *            25*a(n-1) - 245*a(n-2) + 1175*a(n-3) - 2754*a(n-4) + 2520*a(n-5)
 *            Initial values: a(0)=1, a(1)=1, a(2)=-1, a(3)=19, a(4)=467
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)467;

    /* Variables holding the last five values of the sequence */
    seq_t t0 = (seq_t)1;   /* a(n-5) */
    seq_t t1 = (seq_t)1;   /* a(n-4) */
    seq_t t2 = (seq_t)-1;  /* a(n-3) */
    seq_t t3 = (seq_t)19;  /* a(n-2) */
    seq_t t4 = (seq_t)467; /* a(n-1) */
    seq_t tn = 0;

    for (idx_t i = 5; i <= n; ++i) {
        tn = (seq_t)25 * t4 - (seq_t)245 * t3 + (seq_t)1175 * t2
             - (seq_t)2754 * t1 + (seq_t)2520 * t0;
        /* Shift the window of the last five terms */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = tn;
    }
    return tn;
}
