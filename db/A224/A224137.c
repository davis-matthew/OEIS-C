/*
 * A224137  Number of 6 X n 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{10} c_j * binom(n-j+5,6) with c = {64, 281,
 *            -1067, 617, 1387, -1840, 160, 696, -172, -154, 60} This is the
 *            closed form derived from the generating function G(x) = x*(64 +
 *            281x - 1067x^2 + 617x^3 + 1387x^4 - 1840x^5 + 160x^6 + 696x^7 -
 *            172x^8 - 154x^9 + 60x^10) / (1 - x)^7
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 65, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom6(idx_t N)
{
    /* Computes C(N,6) exactly using 128‑bit intermediate arithmetic. */
    unsigned __int128 a = (unsigned __int128)N;
    a *= N - 1;
    a *= N - 2;
    a *= N - 3;
    a *= N - 4;
    a *= N - 5;
    a /= 720;            /* 6! = 720 */
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    /* The array of coefficients c_j from the generating function.   */
    static const int coeffs[11] = {
        64, 281, -1067, 617, 1387, -1840, 160, 696, -172, -154, 60
    };

    if (n == 0) return 0;   /* for safety, a(0)=0 though offset is 1 */

    seq_t sum_pos = 0;  /* sum of positive contributions   */
    seq_t sum_neg = 0;  /* sum of absolute values of negative contributions */

    /* We only need terms up to j = min(n-1, 10).  */
    for (idx_t j = 0; j < 11 && j < n; ++j) {
        idx_t N = n - j + 5;                 /* N ≥ 6 */
        seq_t b = binom6(N);                 /* binom(N,6) */

        if (coeffs[j] >= 0) {
            sum_pos += (seq_t)(coeffs[j]) * b;
        } else {
            /* coeffs[j] is negative; add its absolute value to sum_neg */
            sum_neg += (seq_t)(-coeffs[j]) * b;
        }
    }

    /* Final result: positive part minus negative part.   */
    return sum_pos - sum_neg;
}
