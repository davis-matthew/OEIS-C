/*
 * A173468  Sum n^k, k=0..n+1.
 * https://oeis.org/A173468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n+1} n^k = (n^(n+2)-1)/(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n==1 the geometric formula has division by zero, so use the
       explicit small value. */
    if (n == 1)
        return (seq_t)3;

    seq_t total   = 0;
    seq_t current = 1;          /* n^0 */

    for (idx_t i = 0; i <= n + 1; ++i) {
        total += current;                  /* add n^i */
        if (i < n + 1)
            current *= (seq_t)n;            /* compute n^(i+1) */
    }

    return total;
}
