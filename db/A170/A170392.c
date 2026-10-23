/*
 * A170392  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45 * Σ_{i=1}^{42} a(n-i) - 1035 * a(n-43) + coeff(n)
 *            where coeff(0)=coeff(43)=1, coeff(1..42)=2, coeff(other)=0. This
 *            follows from the rational generating function (1 + 2t + ... +
 *            2t^42 + t^43) / (1 - 45t - ... - 45t^42 + 1035t^43)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const size_t BUF = 43;
    seq_t buf[BUF];
    /* initialize buffer */
    for (size_t i = 0; i < BUF; ++i) buf[i] = (seq_t)0;
    buf[0] = (seq_t)1;   /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* sum of the previous 42 terms (or fewer if i < 42) */
        for (int j = 1; j <= 42; ++j) {
            if ((idx_t)j > i) break;
            size_t idx = (size_t)((i - (idx_t)j) % BUF);
            sum += buf[idx];
        }

        seq_t coeff = (seq_t)0;
        if (i == 43) coeff = (seq_t)1;
        else if (i <= 42) coeff = (seq_t)2;

        seq_t term = 45 * sum;
        if (i >= 43) {
            term -= (1035 * buf[(size_t)((i - 43) % BUF)]);
        }
        term += coeff;

        buf[(size_t)(i % BUF)] = term;
    }

    return buf[(size_t)(n % BUF)];
}
