/*
 * A191496  Number of compositions of even numbers into 9 parts <= n.
 * https://oeis.org/A191496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^9 + (1 + (-1)^n)/2) / 2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 148, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n+1)^9 using a 128‑bit temporary to avoid overflow
       for the range of n that occurs in the benchmarks. */
    unsigned __int128 m = (unsigned __int128)((seq_t)n + 1);
    unsigned __int128 p = 1;
    for (int i = 0; i < 9; ++i)
        p *= m;

    /* Add the parity term: 1 if n is even, 0 if n is odd. */
    unsigned __int128 parity = ((n & 1) == 0) ? 1 : 0;
    p += parity;

    /* Divide by 2 and cast back to seq_t. */
    p /= 2;
    return (seq_t)p;
}
