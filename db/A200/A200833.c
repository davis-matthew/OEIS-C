/*
 * A200833  Number of 0..3 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4) with initial
 *            values: a(1) = 56, a(2) = 194, a(3) = 676, a(4) = 2356 (OEIS
 *            A200833)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset below defined */
    if (n == 1) return (seq_t)56;
    if (n == 2) return (seq_t)194;
    if (n == 3) return (seq_t)676;
    if (n == 4) return (seq_t)2356;

    seq_t a_nm4 = (seq_t)56;   /* a(n-4) */
    seq_t a_nm3 = (seq_t)194;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)676;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)2356; /* a(n-1) */
    seq_t new_val;

    for (idx_t i = 5; i <= n; ++i)
    {
        new_val = 4 * a_nm1 - 2 * a_nm2 + a_nm3 - a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;
}
