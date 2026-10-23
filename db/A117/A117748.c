/*
 * A117748  Triangular numbers divisible by 3.
 * https://oeis.org/A117748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = T_k where k = floor(3*(n-1)/2) + ((n-1)&1) and T_k =
 *            k*(k+1)/2 is the k‑th triangular number. This yields the
 *            triangular numbers divisible by 3, index starting at 1
 * Range:     u8 n <= 15, u16 n <= 241, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to 0‑based index */
    idx_t m = n - 1;

    /* k = floor(3*m/2) + (m & 1)
       Avoid overflow by using shifts: floor(3*m/2) = m + (m>>1) */
    idx_t k = m + (m >> 1) + (m & 1);

    /* Compute the k‑th triangular number in seq_t arithmetic */
    seq_t kk = (seq_t)k;
    return (kk * (kk + 1)) / 2;
}
