/*
 * A192019  The Wiener index of the binary Fibonacci tree of order n.
 * https://oeis.org/A192019
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Wiener index of the binary Fibonacci tree of order n
 *            Recurrence: size[n] = size[n-1] + size[n-2] + 1 S[n] = S[n-1] +
 *            S[n-2] + size[n-1] + size[n-2] // sum of distances W[n] = W[n-1]
 *            + W[n-2] + size[n-2]*S[n-1] + size[n-1]*S[n-2] // cross‑subtree
 *            pairs + 2*size[n-1]*size[n-2] // two edges f...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;          /* values below the offset are undefined */
    }

    /* values for the two previous orders */
    seq_t size_nm2 = (seq_t)0;     /* size[n-2], starts with n=0  */
    seq_t size_nm1 = (seq_t)1;     /* size[n-1], starts with n=1  */
    seq_t S_nm2    = (seq_t)0;     /* sum of distances for n-2 */
    seq_t S_nm1    = (seq_t)0;     /* sum of distances for n-1 */
    seq_t W_nm2    = (seq_t)0;     /* Wiener index for n-2 */
    seq_t W_nm1    = (seq_t)0;     /* Wiener index for n-1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* previous values for this step */
        seq_t s1 = size_nm1;  /* size[n-1]  */
        seq_t s2 = size_nm2;  /* size[n-2]  */
        seq_t S1 = S_nm1;     /* S[n-1]     */
        seq_t S2 = S_nm2;     /* S[n-2]     */
        seq_t W1 = W_nm1;     /* W[n-1]     */
        seq_t W2 = W_nm2;     /* W[n-2]     */

        /* compute values for order i */
        seq_t size_n = s1 + s2 + (seq_t)1;
        seq_t S_n    = S1 + s1 + S2 + s2;
        seq_t W_n    = W1 + W2
                     + s2 * S1
                     + s1 * S2
                     + (seq_t)2 * s1 * s2
                     + size_nm1 + S1
                     + size_nm2 + S2;

        /* roll the window */
        size_nm2 = size_nm1;
        size_nm1 = size_n;
        S_nm2    = S_nm1;
        S_nm1    = S_n;
        W_nm2    = W_nm1;
        W_nm1    = W_n;
    }

    return W_nm1;   /* W[n] */
}
