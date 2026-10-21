/*
 * A078998  Choose a(n) so that a(1)+a(2)+...+a(n) = concatenation of n first
 *          natural numbers.
 * https://oeis.org/A078998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007908(n) - A007908(n-1), with A007908(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* return decimal digit count of an idx_t value */
static inline size_t digits_idx(idx_t x) {
    size_t d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* return 10^e as a seq_t value */
static inline seq_t pow10_seq(size_t e) {
    seq_t v = 1;
    for (size_t i = 0; i < e; ++i) v *= 10;
    return v;
}

seq_t a(idx_t n)
{
    seq_t C = 0;                     /* concatenation A007908(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        size_t d   = digits_idx(i);
        seq_t ten  = pow10_seq(d);
        C = C * ten + (seq_t)i;
    }
    size_t d_n   = digits_idx(n);
    seq_t factor = pow10_seq(d_n) - (seq_t)1;
    return C * factor + (seq_t)n;
}
