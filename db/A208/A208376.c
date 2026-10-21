/*
 * A208376  Number of n X 5 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 4 Initial values:
 *            a(1) = 16, a(2) = 256, a(3) = 704, a(4) = 1344
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return 0;          /* undefined but safe */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;
    if (n == 3) return (seq_t)704;
    if (n == 4) return (seq_t)1344;

    /* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) */
    seq_t a_nm3 = (seq_t)256;   /* a(2) */
    seq_t a_nm2 = (seq_t)704;   /* a(3) */
    seq_t a_nm1 = (seq_t)1344;  /* a(4) */
    seq_t a_n   = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        a_n = 3 * a_nm1 - 3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
