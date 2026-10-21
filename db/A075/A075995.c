/*
 * A075995  a(n) = Sum_{k=1..floor(n/2)} floor(n/k) for n >= 2, with a(1) = 1.
 * https://oeis.org/A075995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1}^{floor(n/2)} floor(n/k) , a(1) = 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;                 /* a(1) = 1 */
    idx_t m = n / 2;                     /* upper limit of k */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= m; ) {
        idx_t q = n / i;                  /* floor(n / i) */
        idx_t next = n / q + 1;           /* first index where floor changes */
        if (next > m + 1)
            next = m + 1;                 /* restrict to the range */
        sum += (seq_t)q * (seq_t)(next - i);
        i = next;
    }
    return sum;
}
