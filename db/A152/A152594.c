/*
 * A152594  a(n) = -5*a(n-1)-2*a(n-2), n>1 ; a(0)=1, a(1)=-1 .
 * https://oeis.org/A152594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -1, a(n) = -5*a(n-1) - 2*a(n-2) for n > 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)-1;
    }

    /* Iteratively compute a(n) using the recurrence.
       Only +, -, *, /, % and simple casts are used, making
       the code valid for both C and C++ and for any implementation
       of seq_t (fixed‑size or arbitrary‑precision). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t an = a1;         /* will hold a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        an = -(seq_t)5 * a1 - (seq_t)2 * a0;  /* a(i) */
        a0 = a1;    /* shift the window */
        a1 = an;
    }

    return an;
}
