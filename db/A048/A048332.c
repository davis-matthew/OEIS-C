/*
 * A048332  Numbers that are repdigits in base 7.
 * https://oeis.org/A048332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 6*floor((n-1)/6)) * (7^floor((n+5)/6) - 1) / 6
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 67, u64 n <= 136,
 *            u128 n <= 272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the equivalent expression a(n) = r * sum_{i=0}^{k-1} 7^i
   where k = floor((n+5)/6) and r = (n-1)%6 + 1.  This avoids overflowing 7^k
   when computing the result with 64‑bit arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute k = floor((n+5)/6) and r = ((n-1) mod 6) + 1. */
    idx_t        k = (n + 5) / 6;                     /* floor((n+5)/6)      */
    const seq_t r = (seq_t)((n - 1) % 6 + 1);          /* 1..6                 */

    /* Sum the geometric series sum_{i=0}^{k-1} 7^i without overflow. */
    seq_t pow  = 1;          /* 7^0                       */
    seq_t sum  = 0;          /* accumulator for the sum   */
    for (idx_t i = 0; i < k; ++i)
    {
        sum += pow;          /* add current power of 7   */
        pow *= (seq_t)7;     /* next power of 7          */
    }

    return sum * r;          /* final term               */
}
