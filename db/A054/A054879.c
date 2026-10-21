/*
 * A054879  Closed walks of length 2n along the edges of a cube based at a
 *          vertex.
 * https://oeis.org/A054879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = (9^n + 3) / 4 (OEIS A054879)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(0)=1, a(1)=3 */
        return (seq_t)(n == 0 ? 1 : 3);

    /* fast exponentiation: compute 9^n */
    seq_t pow9 = (seq_t)1;
    seq_t base = (seq_t)9;
    idx_t exp = n;
    while (exp > 0)
    {
        if (exp & 1u)
            pow9 = pow9 * base;
        base = base * base;
        exp >>= 1;
    }

    /* (9^n + 3) / 4  is always integer */
    return (pow9 + (seq_t)3) / (seq_t)4;
}
