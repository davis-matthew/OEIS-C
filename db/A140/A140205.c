/*
 * A140205  Partial sums of A140085.
 * https://oeis.org/A140205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + popcount(n & 7), a(0) = 0 Equivalently: a(n) =
 *            ((n+1)/8) * 12 + prefix[(n+1) % 8] where prefix =
 *            {0,0,1,2,4,5,7,9} (sum of popcounts for 0..k-1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Prefix sums of popcount(k & 7) for k = 0 .. 7 */
    static const seq_t prefix[8] = {
        (seq_t)0,  /* k = 0   */
        (seq_t)0,  /* k = 1   */
        (seq_t)1,  /* k = 2   */
        (seq_t)2,  /* k = 3   */
        (seq_t)4,  /* k = 4   */
        (seq_t)5,  /* k = 5   */
        (seq_t)7,  /* k = 6   */
        (seq_t)9   /* k = 7   */
    };

    /* m = n + 1, counted in seq_t to avoid overflow of idx_t */
    seq_t m = (seq_t)n + (seq_t)1;

    /* Number of complete 8‑term blocks */
    seq_t q = m / (seq_t)8;
    /* Remainder (0 .. 7) */
    unsigned int r = (unsigned int)(m % (seq_t)8);

    /* a(n) = 12*q + prefix[r]  */
    return q * (seq_t)12 + prefix[r];
}
