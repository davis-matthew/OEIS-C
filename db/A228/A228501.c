/*
 * A228501  Number of n X 3 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically, diagonally or
 *          antidiagonally.
 * https://oeis.org/A228501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3);   a(1)=2, a(2)=3, a(3)=12 */
seq_t a(idx_t n)
{
    /* Guard against indices below the offset – behaviour for n<1 is irrelevant */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Handle the first three terms directly */
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)3;
    if (n == (idx_t)3) return (seq_t)12;

    /* Iteratively apply the recurrence */
    seq_t a_nm3 = (seq_t)2;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)12;  /* a(n-1) */
    seq_t a_n   = (seq_t)0;   /* placeholder for a(n) */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a_n = (a_nm1 * (seq_t)2) + (a_nm2 * (seq_t)3) - (a_nm3 * (seq_t)2);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
