/*
 * A070271  a(n) = n^reverse(n) (ignore leading 0's).
 * https://oeis.org/A070271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070271: a(n) = n^reverse(n)  (reverse of n in decimal, ignoring leading zeros)  */
#include <stdint.h>

static idx_t reverse_digits(idx_t n) {
    /* reverse decimal digits of n, ignore leading zeros in n */
    idx_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;      /* arbitrary, offset starts at 1 */
    idx_t rev = reverse_digits(n);
    return pow_seq_t((seq_t)n, rev);
}
