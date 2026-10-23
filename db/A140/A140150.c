/*
 * A140150  a(1)=1, a(n)=a(n-1)+n^2 if n odd, a(n)=a(n-1)+ n^4 if n is even.
 * https://oeis.org/A140150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=a(n-1)+((n%2)? n^2 : n^4) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t sq = ii * ii;
        if ((i & 1) == 0) {
            sum += sq * sq;   /* n^4 for even n */
        } else {
            sum += sq;        /* n^2 for odd n  */
        }
    }
    return sum;
}
