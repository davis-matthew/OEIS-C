/*
 * A126566  a(0)=1; a(1)=2; a(2)=5; a(3)=14; for n>3: a(n) =
 *          8*a(n-1)-20*a(n-2)+16*a(n-3)-a(n-4).
 * https://oeis.org/A126566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2) + 16*a(n-3) - a(n-4), a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=14
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)14;

    /* Use a sliding window of the last four terms.  The
       variables are named to match the recurrence indices. */
    seq_t a_n_minus_1 = (seq_t)14; /* a(3) */
    seq_t a_n_minus_2 = (seq_t) 5; /* a(2) */
    seq_t a_n_minus_3 = (seq_t) 2; /* a(1) */
    seq_t a_n_minus_4 = (seq_t) 1; /* a(0) */

    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = a_n_minus_1 * (seq_t)8
             - a_n_minus_2 * (seq_t)20
             + a_n_minus_3 * (seq_t)16
             - a_n_minus_4;
        /* Advance the window */
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = next;
    }
    return a_n_minus_1;
}
