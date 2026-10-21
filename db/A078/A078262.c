/*
 * A078262  Sum of the forward and reverse concatenations of 1 to n.
 * https://oeis.org/A078262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000422(n) + A007908(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the forward and reverse concatenations of 1 to n. */

#include "../utils/digit_count.h"
#include "../utils/pow10.h"

/* Returns the n-th term of OEIS sequence A078262. */
seq_t a(idx_t n)
{
    seq_t forward = 0;
    seq_t reverse = 0;
    idx_t k;

    /* Forward concatenation: 1, 2, ..., n */
    for (k = 1; k <= n; ++k) {
        uint64_t d = digit_count(k);          /* number of decimal digits of k */
        seq_t pow = pow10((idx_t)d);          /* 10^d  */
        forward = forward * pow + (seq_t)k;
    }

    /* Reverse concatenation: n, n-1, ..., 1 */
    for (k = n; k > 0; --k) {
        uint64_t d = digit_count(k);
        seq_t pow = pow10((idx_t)d);
        reverse = reverse * pow + (seq_t)k;
    }

    return forward + reverse;
}
