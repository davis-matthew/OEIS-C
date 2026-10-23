/*
 * A033126  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=8, a(3)=65, a(4)=521;
   a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) for n ≥ 5. */
seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)65;
    if (n == 4) return (seq_t)521;

    /* Use a sliding window of the last four computed values. */
    seq_t a_nm4 = (seq_t)1;     /* a(n-4)  */
    seq_t a_nm3 = (seq_t)8;     /* a(n-3)  */
    seq_t a_nm2 = (seq_t)65;    /* a(n-2)  */
    seq_t a_nm1 = (seq_t)521;   /* a(n-1)  */
    seq_t a_n;

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)8 * a_nm1 + a_nm3 - (seq_t)8 * a_nm4;

        /* Slide the window forward by one. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
