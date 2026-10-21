/*
 * A096114  a(1)=1, a(2)=2, a(3*2^k) = 3*2^k, a(3*2^k + i) = 3*2^k + a(3*2^k -
 *          i), for i in range [1, 3*2^k - 1].
 * https://oeis.org/A096114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 9242, u32 n <= 9242, u64 n <= 9242,
 *            u128 n <= 9242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A096114:
   a(1) = 1,  a(2) = 2,
   a(3*2^k) = 3*2^k,
   a(3*2^k + i) = 3*2^k + a(3*2^k - i)   for 1 ≤ i < 3*2^k. */
seq_t a(idx_t n)
{
    /* Handle the two trivial base cases directly. */
    if (n == 1 || n == 2)
        return (seq_t)n;

    seq_t result = 0;

    /* Repeatedly reflect the index until it reaches a base case. */
    for (;;)
    {
        /* A base case is reached when the index equals 1 or 2 */
        if (n == 1 || n == 2)
        {
            result += (seq_t)n;
            return result;
        }

        /* Find the largest m = 3*2^k that does not exceed n. */
        idx_t m = 3;
        while ((m << 1) <= n)
            m <<= 1;

        /* The current block is [m .. 2*m-1].  If n==m, we are at a base
           value and can return directly. */
        if (n == m)
        {
            result += (seq_t)m;
            return result;
        }

        /* For n > m we use the symmetric rule:
           a(n) = m + a(2*m - n).  Accumulate the offset m and update n. */
        result += (seq_t)m;
        n = (m << 1) - n;   /* n = 2*m - n, which is strictly smaller than m */
    }
}
