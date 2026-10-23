/*
 * A117516  Last entry (and high point) in segment n of A079051.
 * https://oeis.org/A117516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117516: Last entry (and high point) in segment n of A079051.
   For n=0..3 the values are fixed. For n≥4:
   if n is even, n = 2k (k≥2):
      a(n) = 5k(k+1) + k(4k^2-1)/3 + 8
   if n is odd, n = 2k+1 (k≥2):
      a(n) = 9k^2 + 9k + (4k^3 - k)/3 + 9
   These formulas produce the sequence 0,3,13,28,48,73,103,152,192,273,...
   meeting the OEIS specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)28;

    if ((n & 1) == 0)    /* even n = 2k, k>=2 */
    {
        idx_t k = n >> 1;                      /* n/2 */
        seq_t ks = (seq_t)k;                   /* cast to seq_t */

        seq_t term1 = (seq_t)5 * ks * (ks + (seq_t)1);
        seq_t term2 = ks * (((seq_t)4 * ks * ks) - (seq_t)1) / (seq_t)3;
        return term1 + term2 + (seq_t)8;
    }
    else                  /* odd n = 2k+1, k>=2 */
    {
        idx_t k = (n - 1) >> 1;                 /* (n-1)/2 */
        seq_t ks = (seq_t)k;

        seq_t term1 = (seq_t)9 * ks * ks;
        seq_t term2 = (seq_t)9 * ks;
        seq_t term3 = ((seq_t)4 * ks * ks * ks - ks) / (seq_t)3;
        return term1 + term2 + term3 + (seq_t)9;
    }
}
