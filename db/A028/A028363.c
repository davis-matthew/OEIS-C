/*
 * A028363  Total number of doubly-even self-dual binary codes of length 8n.
 * https://oeis.org/A028363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*Product_{i=1..4n-2} (2^i + 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t res = (seq_t)2;     /* initial factor 2   */
    seq_t power2 = (seq_t)1;  /* will hold 2^i       */
    idx_t limit = 4 * n - 2;  /* last i in product   */

    for (idx_t i = 1; i <= limit; ++i)
    {
        power2 = power2 * (seq_t)2;          /* power2 = 2^i   */
        res = res * (power2 + (seq_t)1);     /* multiply by 2^i + 1 */
    }

    return res;
}
