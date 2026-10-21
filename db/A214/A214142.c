/*
 * A214142  Number of 0..4 colorings of a 1 X (n+1) array circular in the n+1
 *          direction with new values 0..4 introduced in row major order.
 * https://oeis.org/A214142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) + 8*a(n-4),
   initial values: a(1)=1, a(2)=1, a(3)=4, a(4)=11 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 4;
    if (n == 4) return 11;

    seq_t a_nm4 = 1;   /* a(n-4) */
    seq_t a_nm3 = 1;   /* a(n-3) */
    seq_t a_nm2 = 4;   /* a(n-2) */
    seq_t a_nm1 = 11;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = 6 * a_nm1 - 7 * a_nm2 - 6 * a_nm3 + 8 * a_nm4;

        /* shift the window for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;  /* a(n) */
}
