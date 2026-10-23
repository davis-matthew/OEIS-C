/*
 * A195235  Number of lower triangles of a 5 X 5 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by one or less.
 * https://oeis.org/A195235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) for n>4 with a(1)=32768, a(2)=127785,
 *            a(3)=237818, a(4)=348849
 * Range:     u8 none, u16 n <= 1, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)32768;
    if (n == 2) return (seq_t)127785;
    if (n == 3) return (seq_t)237818;
    if (n == 4) return (seq_t)348849;

    /* For n > 4 use the linear recurrence */
    seq_t a_nm2 = (seq_t)237818;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)348849;   /* a(n-1)  */
    seq_t curr   = 0;              /* a(n)    */

    /* Iterate from 5 up to the desired n */
    for (idx_t i = 5; i <= n; ++i) {
        curr = a_nm1 * ((seq_t)2) - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return curr;
}
