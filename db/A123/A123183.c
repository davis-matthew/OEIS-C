/*
 * A123183  a(1)=-1; a(2)=-1; a(3)=-2; a(n) = 4*a(n-1) - 3*a(n-2) for n >= 4.
 * https://oeis.org/A123183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = -1, a(2) = -1, a(3) = -2, a(n) = 4*a(n-1) - 3*a(n-2) for n ≥ 4 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)-2;

    /* a(n-2) and a(n-1) for the current step (start with n=4) */
    seq_t a_nm2 = (seq_t)-1;   /* a(2) */
    seq_t a_nm1 = (seq_t)-2;   /* a(3) */
    seq_t a_n   = a_nm1;       /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n   = 4 * a_nm1 - 3 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
