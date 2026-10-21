/*
 * A070624  a(n) = n^5 mod 41.
 * https://oeis.org/A070624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 41
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic modulo 41 with period 41, so n^5 mod 41 can be computed
   using the remainder of n divided by 41. */
seq_t a(idx_t n)
{
    /* Reduce n modulo 41 to avoid large intermediate values */
    idx_t r = n % 41ULL;           /* r is in [0,40] */
    seq_t res = 1;
    seq_t mod = (seq_t)r;          /* cast once to seq_t before multiplying */
    /* Compute r^5 modulo 41 */
    for (int i = 0; i < 5; ++i)
    {
        res = (res * mod) % 41;
    }
    return res;
}
