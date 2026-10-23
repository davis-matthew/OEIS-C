/*
 * A100215  Expansion of (4 - 7*x + 2*x^2)/((1-2*x)*(1 - 2*x + 2*x^2)).
 * https://oeis.org/A100215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3), a(0)=4, a(1)=9, a(2)=14
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)14;

    /* Iterate using the linear recurrence. */
    seq_t a_nm3 = (seq_t)4;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)9;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)14;  /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 4 * a_nm1 - 6 * a_nm2 + 4 * a_nm3;
        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
