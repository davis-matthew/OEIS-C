/*
 * A020975  Expansion of 1/((1-7*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A020975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 293*a(n-2) + 924*a(n-3) with a(0) = 1, a(1) =
 *            30, a(2) = 607
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A020975. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)607;

    seq_t a_nm3 = (seq_t)1;    /* a(n-3)  */
    seq_t a_nm2 = (seq_t)30;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)607;  /* a(n-1)  */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 30 * a_nm1 + 924 * a_nm3 - 293 * a_nm2;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
