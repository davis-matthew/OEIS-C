/*
 * A129889  Write down n, then n*(n+1).
 * https://oeis.org/A129889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n is even, a(n) = ((n-1)/2) * ((n+1)/2) if n is
 *            odd
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even indices: a(2k) = k.  Odd indices: a(2k+1) = k*(k+1). */
    if ((n & 1) == 0) /* even */
        return ((seq_t)n) / 2;
    else {
        idx_t k = n >> 1;                 /* k = floor((n-1)/2) */
        return ((seq_t)k) * (k + 1);      /* k * (k+1) */
    }
}
