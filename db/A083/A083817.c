/*
 * A083817  Interleaving of (1,2,3,4,5,...), (2,2,2,2,...) and
 *          (0,1,3,6,10,...).
 * https://oeis.org/A083817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k)=k+1, a(3k+1)=2, a(3k+2)=k*(k+1)/2
 * Range:     u8 n <= 70, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 1)          /* indices 3k+1 */
        return (seq_t)2;
    else if (r == 0)     /* indices 3k   */
        return (seq_t)(n / 3 + 1);
    else                 /* indices 3k+2 */
    {
        seq_t k = (seq_t)(n - 2) / 3;  /* k = (n-2)/3 >= 0 */
        return k * (k + (seq_t)1) / (seq_t)2;
    }
}
