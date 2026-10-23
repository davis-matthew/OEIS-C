/*
 * A136545  a(n) = Sum_{k=1..floor(n*log(2))} (1 - (n - floor(n/2^k)) * (n -
 *          floor(n/2^(k - 1))) mod 2).
 * https://oeis.org/A136545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Σ_{k=1..⌊n*log(2)⌋} (1 – ((n – ⌊n/2^k⌋)·(n – ⌊n/2^{k‑1}⌋)
 *            mod 2))
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const double LOG2_CONST = 0.6931471805599453; /* natural logarithm of 2 */

seq_t a(idx_t n)
{
    /* compute m = floor(n * log(2)) */
    idx_t m = (idx_t)(n * LOG2_CONST);

    seq_t sum = 0;

    /* f_k_minus1 = floor(n / 2^{k-1}) , f_k = floor(n / 2^k) */
    idx_t f_k_minus1 = n;
    idx_t f_k = n >> 1;          /* k = 1 */

    for (idx_t k = 1; k <= m; ++k) {
        idx_t r1 = n - f_k;          /* n - floor(n/2^k)   */
        idx_t r2 = n - f_k_minus1;    /* n - floor(n/2^{k-1}) */

        /* product mod 2 is 1 iff both r1 and r2 are odd */
        seq_t parity = (seq_t)((r1 & 1) & (r2 & 1));
        sum += (seq_t)(1 - parity);   /* term = 1 - parity */

        /* advance to k+1 */
        f_k_minus1 = f_k;
        f_k >>= 1;                    /* floor(n/2^{k+1})  */
    }

    return sum;
}
