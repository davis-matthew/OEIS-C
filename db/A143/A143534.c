/*
 * A143534  Number of decimal digits in the high-water marks of the terms of
 *          the continued fraction of the (base-10) Champernowne constant.
 * https://oeis.org/A143534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   * For n ≥ 4, a(n) = NCD(n) - 2·NCD(n‑1) - 3·(n‑2) + 4 * where
 *            NCD(n) = 3 – n + 9·∑_{m=1}^{n‑3} m·10^{m‑1} (n ≥ 3) * Initial
 *            values: * a(1) = 0, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

 
static seq_t compute_NCD(idx_t n) {
    /* n must be ≥ 4;  for n = 3 we return 0 directly elsewhere */
    seq_t pow10_n_minus_3 = pow10(n - 3);          /* 10^(n-3)  */
    seq_t pow10_n_minus_2 = pow10_n_minus_3 * (seq_t)10;   /* 10^(n-2)  */
    seq_t coeff = (seq_t)(9 * n - 28);            /* (9n-28), ≥ 0 for n ≥ 4 */
    seq_t term = pow10_n_minus_2 * coeff;         /* 10^(n-2)·(9n-28) */
    term += (seq_t)10;
    term /= (seq_t)90;                            /* (10 + (9n-28)·10^(n-2)) / 90 */
    seq_t result = term + (seq_t)3 - (seq_t)n;    /* 3 – n + … */
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    seq_t NCD_n   = compute_NCD(n);
    seq_t NCD_nm1;
    if (n - 1 == 3) {
        NCD_nm1 = (seq_t)0;          /* NCD(3) = 0 */
    } else {
        NCD_nm1 = compute_NCD(n - 1);
    }

    seq_t result = NCD_n - (seq_t)2 * NCD_nm1;
    result -= (seq_t)3 * (n - 2);
    result += (seq_t)4;
    return result;
}
