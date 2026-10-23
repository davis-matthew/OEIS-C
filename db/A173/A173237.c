/*
 * A173237  Alternate k and 10k for each k.
 * https://oeis.org/A173237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Alternate k and 10*k for each k (OEIS A173237)
   a(n) = 5*n          for even n
          (n+1)/2      for odd n   (with n starting at 1) */
seq_t a(idx_t n)
{
    /* Even indices -> 10*k (k=n/2), which equals 5*n */
    if ((n & 1U) == 0U)
        return (seq_t)5 * (seq_t)n;
    /* Odd indices -> k ((k = (n+1)/2)) */
    return ((seq_t)n + 1) / 2;
}
