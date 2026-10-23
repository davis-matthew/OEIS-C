/*
 * A023550  Convolution of natural numbers >= 2 and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+4) - 2*n - 7 (computes Lucas number via the
 *            recurrence L_k = L_{k-1} + L_{k-2}, with L_0 = 2, L_1 = 1. The
 *            result is then shifted by 2*n + 7.)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute L_{n+4} */
    idx_t k = n + 4;
    seq_t L_prev = 2;   /* L_0   */
    seq_t L_curr = 1;   /* L_1   */

    for (idx_t i = 2; i <= k; ++i) {
        seq_t L_next = L_prev + L_curr;
        L_prev = L_curr;
        L_curr = L_next;
    }

    seq_t lucas = L_curr;                    /* L_{n+4}   */
    seq_t two_n = (seq_t)2 * (seq_t)n;        /* 2*n       */
    seq_t res   = lucas - two_n - (seq_t)7;   /* Lucas - 2*n - 7 */

    return res;
}
