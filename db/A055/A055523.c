/*
 * A055523  Longest other leg of a Pythagorean triangle with n as length of a
 *          leg.
 * https://oeis.org/A055523
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if (n even) then (n/2 - 1) * (n/2 + 1) * if (n odd) then
 *            2 * ((n-1)/2) * (((n-1)/2) + 1) * (This follows the closed‑form
 *            a(n) = (3*(n^2-2)+(-1)^(n+1)*(n^2+2))/8)
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for internal arithmetic */
    seq_t N = (seq_t)n;

    /* Even n: a(2k) = k^2 - 1 = (k-1)*(k+1) with k = N/2 */
    if ((n & 1) == 0) {
        seq_t k = N / 2;
        return (k - 1) * (k + 1);
    }

    /* Odd n: a(2k+1) = 2k(k+1) with k = (N-1)/2 */
    seq_t k = (N - 1) / 2;
    return k * (k + 1) * 2;
}
