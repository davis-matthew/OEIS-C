/*
 * A140159  a(1)=1, a(n) = a(n-1) + n^4 if n odd, a(n) = a(n-1) + n^2 if n is
 *          even.
 * https://oeis.org/A140159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2, a(n)=a(n-1)+n^4 if n odd, a(n-1)+n^2 if n even
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t acc = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        if (i & 1) {            /* n odd: add n^4 */
            seq_t sq = ii * ii; /* n^2 */
            acc += sq * sq;     /* n^4  = (n^2)^2 */
        } else {                /* n even: add n^2 */
            acc += ii * ii;
        }
    }
    return acc;
}
