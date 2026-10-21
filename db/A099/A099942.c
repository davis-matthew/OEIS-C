/*
 * A099942  Start with 1, then alternately double or add 2.
 * https://oeis.org/A099942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   for n > 0, a(n) = a(n-1) * 2  if n is odd,
                    a(n-1) + 2  if n is even. */
seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1U)   /* n is odd */
            res = res + res;   /* multiply by 2 */
        else
            res += 2ULL;      /* add 2 */
    }
    return res;
}
