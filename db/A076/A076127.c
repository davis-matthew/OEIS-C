/*
 * A076127  n-th term is binary string of length t_n with 1's at positions
 *          t_i, where t_n = n-th triangular number.
 * https://oeis.org/A076127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10^n * a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the n-th term of OEIS A076127. */
seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t result = 0;
    seq_t power = 10;          /* power = 10^1 initially */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * power + 1; /* result = 10^i * result + 1 */
        power *= 10;                 /* power = 10^(i+1) for next loop */
    }
    return result;
}
