/*
 * A212500  a(n) is the difference between multiples of 5 with even and odd
 *          digit sum in base 4 in interval [0,4^n).
 * https://oeis.org/A212500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Recurrence for A212500 a(n) = 10*a(n-2) - 5*a(n-4) (n >= 5)
 *            initial values: a(1)=1, a(2)=4, a(3)=7, a(4)=36
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 7;
    if (n == 4) return 36;

    /* rolling buffer for a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = 1;   /* a(1)   */
    seq_t a_nm3 = 4;   /* a(2)   */
    seq_t a_nm2 = 7;   /* a(3)   */
    seq_t a_nm1 = 36;  /* a(4)   */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = a_nm2 * 10 - a_nm4 * 5;
        /* shift the buffer */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
