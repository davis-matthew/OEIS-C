/*
 * A139745  a(n) = 11^n - 7^n.
 * https://oeis.org/A139745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 7^n Computation via the linear recurrence a(0) =
 *            0, a(1) = 4, a(n) = 18*a(n-1) - 77*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)  return (seq_t)0;
    if (n == 1u)  return (seq_t)4;

    seq_t a_nm2 = (seq_t)0;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)4;   /* a(n-1)  */
    seq_t a_n   = 0;

    for (idx_t i = 2u; i <= n; ++i) {
        a_n = (seq_t)18 * a_nm1 - (seq_t)77 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
