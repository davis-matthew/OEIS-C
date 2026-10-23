/*
 * A142586  Binomial transform of A014217.
 * https://oeis.org/A142586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=14; a(n) = 5*a(n-1) - 7*a(n-2) +
 *            2*a(n-3) for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)14;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)2; /* a(1) */
    seq_t a_nm1 = (seq_t)5; /* a(2) */
    seq_t a_n   = (seq_t)14;/* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = 5 * a_n - 7 * a_nm1 + 2 * a_nm2;
        a_nm2 = a_nm1;   /* shift for next step */
        a_nm1 = a_n;
        a_n   = new_val;
    }
    return a_n;
}
