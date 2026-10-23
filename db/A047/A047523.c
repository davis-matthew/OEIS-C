/*
 * A047523  Numbers that are congruent to {0, 1, 7} mod 8.
 * https://oeis.org/A047523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 8*k-1, a(3k-1) = 8*k-7, a(3k-2) = 8*k-8
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A047523 (numbers congruent to {0,1,7} mod 8). */
seq_t a(idx_t n)
{
    /* n is 1-based; n>=1 is assumed */
    idx_t m = n % 3;
    seq_t result;

    if (m == 0) /* n = 3k */
    {
        result = (seq_t)8 * (seq_t)(n / 3) - 1;
    }
    else if (m == 1) /* n = 3k-2  =>  k = (n+2)/3 */
    {
        result = (seq_t)8 * (seq_t)((n + 2) / 3) - 8;
    }
    else /* m == 2, n = 3k-1  =>  k = (n+1)/3 */
    {
        result = (seq_t)8 * (seq_t)((n + 1) / 3) - 7;
    }

    return result;
}
