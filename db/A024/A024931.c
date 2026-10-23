/*
 * A024931  a(n) = sum of remainders of n mod 2,4,6,...,2k, where k = [ n/2 ].
 * https://oeis.org/A024931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..floor((n-1)/2)} ((n-1) mod (2*i))
 * Range:     u8 n <= 55, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS offset 2: a(2) is the first printed term) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* we consider the sum for m = n-1 */
    seq_t m   = (seq_t)n - 1;
    idx_t   k = (n - 1) >> 1;          /* floor((n-1)/2) */

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t d = (seq_t)(i << 1);     /* d = 2*i */
        sum += m % d;
    }

    return sum;
}
