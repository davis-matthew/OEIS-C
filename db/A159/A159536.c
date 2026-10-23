/*
 * A159536  Numerator of Hermite(n, 8/17).
 * https://oeis.org/A159536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17^n * Hermite(n, 8/17) Recurrence: a(0) = 1, a(1) = 16,
 *            a(n+1) = 16*a(n) - 578*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)16;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)16;  /* a(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        seq_t term = (seq_t)578 * (seq_t)i;   /* 578 * i */
        term = term * a_nm2;                  /* 578 * i * a(n-2) */
        seq_t a_next = (seq_t)16 * a_nm1 - term; /* 16*a(n-1) - term */
        a_nm2 = a_nm1;
        a_nm1 = a_next;
    }
    return a_nm1;
}
