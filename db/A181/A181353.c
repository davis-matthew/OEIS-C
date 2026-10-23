/*
 * A181353  a(n) = 9*a(n-1) + 3*a(n-2); a(0) = 0, a(1) = 1.
 * https://oeis.org/A181353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 3*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    if (n == 1u)
        return 1;

    seq_t a_nm2 = 0;  /* a(n-2) */
    seq_t a_nm1 = 1;  /* a(n-1) */
    seq_t an   = 0;   /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = ((seq_t)9 * a_nm1) + ((seq_t)3 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
