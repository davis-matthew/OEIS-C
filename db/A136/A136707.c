/*
 * A136707  At step n the sequence lists the number of occurrences of digit (n
 *          mod k), with k>0, in all the numbers from 1 to n. Case k=3.
 * https://oeis.org/A136707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of occurrences of digit (n+1 mod 3) in all decimal
 *            numbers from 1 to n+1 (offset 0)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1‑based upper bound */
    idx_t N = n + 1u;
    /* digit to count */
    int d = (int)(N % 3u);

    seq_t cnt = 0;

    for (idx_t i = 1u; i <= N; ++i)
    {
        idx_t x = i;
        while (x > 0u)
        {
            if ((x % 10u) == (idx_t)d)
                ++cnt;
            x /= 10u;
        }
    }

    return cnt;
}
