/*
 * A140146  a(1)=1, a(n)=a(n-1)+n^1 if n odd, a(n)=a(n-1)+ n^4 if n is even.
 * https://oeis.org/A140146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) + n^1 if n is odd, a(n) =
 *            a(n-1) + n^4 if n is even
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 133, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t sum = 1;

    for (idx_t k = 2; k <= n; ++k)
    {
        if (k & 1U)            /* odd */
            sum += (seq_t)k;   /* n^1 */
        else
        {
            seq_t kk = (seq_t)k;
            sum += kk * kk * kk * kk; /* n^4 */
        }
    }

    return sum;
}
