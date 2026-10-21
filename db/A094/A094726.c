/*
 * A094726  Let M = the 2 X 2 matrix [ 0 3 / 3 2]. Take (M^n * [1 1])/3 = [p
 *          q]; then a(n) = p.
 * https://oeis.org/A094726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 9*a(n-2), a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;    /* a(n-2) */
    seq_t a_nm1 = (seq_t)5;    /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_n = ( (seq_t)2 * a_nm1 ) + ( (seq_t)9 * a_nm2 );
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
