/*
 * A154247  a(n) = ( (6 + sqrt(7))^n - (6 - sqrt(7))^n )/(2*sqrt(7)).
 * https://oeis.org/A154247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 29*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t1 = a1 * (seq_t)12;
        seq_t t2 = a0 * (seq_t)29;
        seq_t cur = t1 - t2;  /* t1 >= t2 always */
        a0 = a1;
        a1 = cur;
    }

    return a1;
}
