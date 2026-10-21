/*
 * A215097  a(n) = n^3 - a(n-2) for n >= 2 and a(0)=0, a(1)=1.
 * https://oeis.org/A215097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_nm2 = 0; /* a(0) */
    seq_t a_nm1 = 1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t idx = (seq_t)i;
        seq_t cube = idx * idx * idx;
        seq_t a_n = cube - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1; /* a(n) */
}
