/*
 * A083304  a(n) = (4*(n+5)^n + n^n)/5.
 * https://oeis.org/A083304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*(n+5)^n + n^n)/5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast power in seq_t type */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* (n+5)^n */
    seq_t p1 = pow_seq_t((seq_t)(n + 5), n);
    /* n^n */
    seq_t p2 = pow_seq_t((seq_t)n, n);
    /* (4*(n+5)^n + n^n)/5 */
    seq_t sum = p1 * (seq_t)4 + p2;
    return sum / (seq_t)5;
}
