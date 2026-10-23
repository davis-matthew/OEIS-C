/*
 * A174989  Partial sums of A003602.
 * https://oeis.org/A174989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} (popcount(i)+1)/2 Recurrence used: a(0)=0;
 *            a(n)=tri(ceil(n/2)) + a(floor(n/2)) where tri(m)=m*(m+1)/2. This
 *            is implemented iteratively
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    while (n != 0)
    {
        idx_t left = (n + 1) >> 1;                  /* ceil(n/2)   */
        __int128 prod = (__int128)left * (left + 1);/* left*(left+1) */
        seq_t tri = (seq_t)(prod >> 1);             /* triangular number */
        result += tri;
        n >>= 1;                                   /* floor(n/2)   */
    }
    return result;
}
