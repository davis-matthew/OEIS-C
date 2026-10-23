/*
 * A037835  Sum{|d(i)-d(i-1)|: i=0,1,...,m}, where Sum{d(i)*3^i: i=0,1,...,m}
 *          is base 3 representation of n.
 * https://oeis.org/A037835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i=1}^{m} |d_i - d_{i-1}|, where n = Σ d_i·3^i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* the sequence is defined for n >= 1 */

    /* Least significant base‑3 digit */
    idx_t prev = n % 3;
    idx_t r    = n / 3;
    seq_t sum  = 0;

    while (r > 0) {
        idx_t curr = r % 3;
        /* Compute |curr - prev| without using signed arithmetic */
        sum += (curr > prev) ? (seq_t)(curr - prev) : (seq_t)(prev - curr);
        prev = curr;
        r   /= 3;
    }
    return sum;
}
