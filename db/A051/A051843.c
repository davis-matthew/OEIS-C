/*
 * A051843  Partial sums of A002419.
 * https://oeis.org/A051843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(6*n-1)/120
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 152, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit arithmetic so that all intermediate
       values overflow safely for 64‑bit builds. */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)n;
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(6 * (unsigned __int128)n - 1);
    prod /= 120;
    return (seq_t)prod;
}
