/*
 * A196655  a(n) = 33*2^(n+1) + 7.
 * https://oeis.org/A196655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*2^(n+1) + 7
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative recurrence: a(0)=73, a(1)=139, a(n)=3*a(n-1)-2*a(n-2) */

seq_t a(idx_t n) {
    if (n == 0) return 73;
    if (n == 1) return 139;
    seq_t a_nm2 = 73;   /* a(n-2)  */
    seq_t a_nm1 = 139;  /* a(n-1)  */
    seq_t a_n;
    for (idx_t i = 2; i <= n; ++i) {
        /* a(n) = 3*a(n-1) - 2*a(n-2) */
        a_n = a_nm1 * 3 - a_nm2 * 2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
