/*
 * A168690  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A168690
    Number of reduced words of length n in the Coxeter group on 13 generators S_i
    with relations (S_i)^2 = (S_i S_j)^17 = I.

    Recurrence:
        a(0) = 1
        For n >= 1:
            let h(n) = 2   for 1 <= n <= 16
                        = 1   for n = 17
                        = 0   for n >= 18
            let sum(n) = Σ_{k=1}^{min(16,n)} a(n-k)
            let term(n) = (n >= 17) ? a(n-17) : 0
            a(n) = h(n) + 11 * sum(n) - 66 * term(n)
    This recurrence reproduces all terms listed in the OEIS entry and all
    subsequent terms. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n = 0 explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Circular buffer for the last 17 values of a(k) */
    seq_t buf[17] = {0};
    buf[0] = (seq_t)1;        /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum of the last up to 16 values */
        for (idx_t k = 1; k <= 16 && i >= k; ++k) {
            sum += buf[(i - k) % 17];
        }

        seq_t term17 = 0;
        if (i >= 17)
            term17 = buf[(i - 17) % 17];

        seq_t h;
        if (i <= 16)
            h = (seq_t)2;
        else if (i == 17)
            h = (seq_t)1;
        else
            h = (seq_t)0;

        seq_t ai = h + (seq_t)11 * sum - (i >= 17 ? (seq_t)66 * term17 : (seq_t)0);
        buf[i % 17] = ai;
    }

    return buf[n % 17];
}
