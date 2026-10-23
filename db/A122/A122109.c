/*
 * A122109  a(n) = 9*a(n-2) - 4*a(n-3) for n > 2 with a(0)=1, a(1)=2.
 * https://oeis.org/A122109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 40, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-2) - 4*a(n-3) with a(0)=1, a(1)=2, a(2)=6 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 6;

    seq_t a_nm3 = 1; /* a(n-3) */
    seq_t a_nm2 = 2; /* a(n-2) */
    seq_t a_nm1 = 6; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)9 * a_nm2 - (seq_t)4 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
