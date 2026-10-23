/*
 * A126876  Numbers of unstrained alkane staggered conformers (acyclic). See
 *          Table 4 of Cyvin et al. reference for precise definition.
 * https://oeis.org/A126876
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 1, with a(5)=1, a(6)=3
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return 0;            /* below offset: value is undefined */
    if (n == 5) return 1;
    if (n == 6) return 3;
    /* compute iteratively using the recurrence */
    seq_t a_nm2 = 1; /* a(5)   */
    seq_t a_nm1 = 3; /* a(6)   */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t a_n = 2 * a_nm1 + a_nm2 + 1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
