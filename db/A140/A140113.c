/*
 * A140113  a(1)=1, a(n)=a(n-1)+n if n odd, a(n)=a(n-1)+ n^2 if n is even.
 * https://oeis.org/A140113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=a(n-1)+n if n odd, a(n-1)+n^2 if n even
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0, but keep safe */
    seq_t sum = (seq_t)1;             /* a(1) = 1 */
    if (n == 1) return sum;

    for (idx_t i = 2; i <= n; ++i) {
        if (i & 1) {                  /* odd i */
            sum += (seq_t)i;
        } else {                      /* even i */
            seq_t ii = (seq_t)i;
            sum += ii * ii;           /* add i^2 */
        }
    }
    return sum;
}
