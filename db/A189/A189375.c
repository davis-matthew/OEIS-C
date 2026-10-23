/*
 * A189375  Expansion of 1/((1-x)^5*(x^3+x^2+x+1)^3).
 * https://oeis.org/A189375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (2*n^4+56*n^3+538*n^2+2044*n+2469+3*((2*n^2+28*n+89)*(-1)^n
 *            +4*(-1)^((2*n-1+(-1)^n)/4)*(n^2+16*n+57-(n^2+12*n+29)*(-1)^n)))/3072.
 *            The implementation below evaluates the right‑hand side using
 *            only integer arithmetic; the division by 3072 is exact
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Work with signed 64‑bit intermediates – for n up to ~10,000 all
       intermediate values fit comfortably inside int64_t. */
    int64_t nn = (int64_t)n;
    int64_t n2 = nn * nn;          /* n^2   */
    int64_t n3 = n2 * nn;          /* n^3   */
    int64_t n4 = n2 * n2;          /* n^4   */

    /* (-1)^n */
    int64_t sign_n = (nn & 1) ? -1 : 1;
    /* (-1)^{floor(n/2)}  with floor(n/2) = n/2 using integer division */
    int64_t e = nn / 2;
    int64_t sign_e = (e & 1) ? -1 : 1;

    /* Compute the inner expression step by step */
    int64_t A = (2 * n2 + 28 * nn + 89) * sign_n;
    int64_t B = n2 + 16 * nn + 57 - (n2 + 12 * nn + 29) * sign_n;
    int64_t inner = A + 4 * sign_e * B;

    /* Numerator of the closed form */
    int64_t numerator =
        2 * n4 + 56 * n3 + 538 * n2 + 2044 * nn + 2469
        + 3 * inner;

    /* Final value – the division is exact */
    seq_t result = (seq_t)(numerator / 3072);
    return result;
}
