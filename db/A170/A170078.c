/*
 * A170078  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 21; a(n) = 20 * a(n-1) for 2 ≤ n ≤ 36; a(n) =
 *            Σ_{i=1}^{36} a(n-i) – 10 * a(n-37) for n ≥ 37
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t buf[37];          /* holds the last 37 terms, oldest at index pos */
    static seq_t sum_all;          /* Σ of elements currently in buf */
    static idx_t pos = 0;          /* index of the oldest element in buf */

    if (!initialized) {
        buf[0] = (seq_t)1;                      /* a(0) */
        buf[1] = (seq_t)21;                     /* a(1) */
        for (idx_t i = 2; i < 37; ++i)          /* a(2) … a(36) = 20·prev */
            buf[i] = buf[i - 1] * (seq_t)20;

        sum_all = 0;
        for (idx_t i = 0; i < 37; ++i)
            sum_all += buf[i];

        pos = 0;
        initialized = 1;
    }

    if (n < 37)
        return buf[n];

    for (idx_t i = 37; i <= n; ++i) {
        seq_t oldest = buf[pos];
        seq_t new_val = sum_all - oldest * (seq_t)11;   /* a(i) = Σ – 10·a(i-37) */
        sum_all = sum_all - oldest + new_val;
        buf[pos] = new_val;
        pos = (pos + 1) % 37;
    }

    /* After the loop, the newest element a(n) is located at (pos + 36) % 37 */
    return buf[(pos + 36) % 37];
}
