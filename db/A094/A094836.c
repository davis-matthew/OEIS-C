/*
 * A094836  a(n) = 2702*a(n-1) - a(n-2); a(-1) = -15; a(0) = 15.
 * https://oeis.org/A094836
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2702*a(n-1) - a(n-2), a(-1) = -15, a(0) = 15
 * Range:     i8 n <= 0, i16 n <= 0, i32 n <= 2, i64 n <= 5, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the recurrence iteratively.  For n < -1 the function returns
 * the last defined term a(-1); behavior below the offset is unspecified
 * as required. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == -1)
        return (seq_t)(-15);
    if (n == 0)
        return (seq_t)(15);

    /* If the requested index is negative but not -1, fall back to -1. */
    if (n < -1)
        return (seq_t)(-15);

    /* Iterate from n = 1 up to the desired n */
    seq_t a_nm2 = (seq_t)(-15);  /* a(-1) */
    seq_t a_nm1 = (seq_t)(15);   /* a(0) */
    seq_t a_n   = a_nm1;         /* placeholder */

    for (idx_t i = 1; i <= n; ++i) {
        a_n = (seq_t)2702 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
