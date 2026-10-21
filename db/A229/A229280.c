/*
 * A229280  Number of ascending runs in {1,...,6}^n.
 * https://oeis.org/A229280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=6, a(2)=57, a(n)=12*a(n-1)-36*a(n-2) for n>2 */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 6;
    if (n == 2)
        return 57;

    seq_t a_nm2 = 6;   /* a(1) */
    seq_t a_nm1 = 57;  /* a(2) */
    seq_t result = 0;

    for (idx_t i = 3; i <= n; ++i) {
        result = 12 * a_nm1 - 36 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = result;
    }

    return result;
}
