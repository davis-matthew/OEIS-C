/*
 * A133146  Antidiagonal sums of the triangle A133128.
 * https://oeis.org/A133146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = (19 + 10*n + 6*n^2 - ((n%2==0)?(2*n+3):-(2*n+3))) / 8
 * Range:     u8 n <= 17, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the explicit closed‑form from the OEIS entry. */
seq_t a(idx_t n)
{
    /* use 64‑bit arithmetic (idx_t) to avoid overflow for n up to at least 10^6 */
    idx_t numerator = 19 + 10 * n + 6 * n * n;

    /* adjust for the (-1)^n term: subtract when n even, add when n odd */
    if ((n & 1) == 0)
    {
        numerator -= 2 * n + 3;
    }
    else
    {
        numerator += 2 * n + 3;
    }

    /* the result is guaranteed to be divisible by 8 */
    return (seq_t)(numerator / 8);
}
