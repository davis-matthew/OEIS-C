/*
 * A222467  Denominator sequence of the n-th convergent of the continued
 *          fraction 1/(1 + 2/(2 + 2/(3 + 2/(4 + ...
 * https://oeis.org/A222467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(-1)=0, for n≥1  a(n)=n·a(n-1)+2·a(n-2) */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t a_nm2 = (seq_t)0; /* a(-1) */
    seq_t a_nm1 = (seq_t)1; /* a(0)  */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_n = ((seq_t)i) * a_nm1 + ((seq_t)2) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
