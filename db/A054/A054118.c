/*
 * A054118  Subdiagonal T(n,n-3), array T as in A054115.
 * https://oeis.org/A054118
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - n*a(n-2) for n >= 6 Initial values:
 *            a(3)=1, a(4)=24, a(5)=144
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0 */
    if (n < 3)
        return (seq_t)0;

    /* Base cases */
    if (n == 3)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)24;
    if (n == 5)
        return (seq_t)144;

    /* Iteratively compute via the recurrence:
       a(6) uses a(5)=144 and a(4)=24 */
    seq_t a_nm2 = (seq_t)24;   /* a(4)  */
    seq_t a_nm1 = (seq_t)144;  /* a(5)  */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 6; i <= n; ++i) {
        a_n = (seq_t)(i + 1) * a_nm1 - (seq_t)i * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
