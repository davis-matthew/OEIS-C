/*
 * A094254  Let M be the 3 X 3 matrix [ 6 0 -8 / 1 0 0 / 0 1 0]. Then M^n * [1
 *          1 1] = [a(n-1), a(n), a(n+1)].
 * https://oeis.org/A094254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-3) for n>=5 Initial values: a(1)=1,
 *            a(2)=2, a(3)=20, a(4)=128
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)128;

    /* Sliding window of three consecutive terms:
       a_nm3  = a(n-3),  a_nm2 = a(n-2),  a_nm1 = a(n-1) */
    seq_t a_nm3 = (seq_t)2;   /* a(2) */
    seq_t a_nm2 = (seq_t)20;  /* a(3) */
    seq_t a_nm1 = (seq_t)128; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)6 * a_nm1 - (seq_t)8 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
