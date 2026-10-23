/*
 * A045643  Palindromic and divisible by 8.
 * https://oeis.org/A045643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 86, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045643: palindromic numbers divisible by 8.  The sequence is generated
   by enumerating all decimal palindromes in ascending order and selecting
   those that are divisible by 8.  The first term (index 1) is 0. */

#include <stdbool.h>

/* reverse the decimal digits of a value */
static inline seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x   = x / (seq_t)10;
    }
    return rev;
}

/* return the n-th a(n) value */
seq_t a(idx_t n)
{
    /* precompute powers of ten up to a reasonably large length */
    static seq_t pow10[64];
    static bool initialized = false;

    if (!initialized) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 64; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        initialized = true;
    }

    if (n == 1)      /* the first term is 0 */
        return (seq_t)0;

    /* count already includes the term 0 */
    idx_t count = 1;

    /* length 1 palindromes: 1–9 */
    for (int d = 1; d <= 9; ++d) {
        seq_t val = (seq_t)d;
        if (val % (seq_t)8 == 0) {
            ++count;
            if (count == n)
                return val;
        }
    }

    /* palindromes of length >= 2 */
    for (int len = 2; ; ++len) {
        if (len % 2 == 0) {              /* even length */
            int half = len / 2;
            seq_t start = pow10[half - 1];
            seq_t end   = pow10[half] - (seq_t)1;

            for (seq_t i = start; i <= end; ++i) {
                seq_t rev = reverse_digits(i);
                seq_t val = i * pow10[half] + rev;
                if (val % (seq_t)8 == 0) {
                    ++count;
                    if (count == n)
                        return val;
                }
            }
        } else {                         /* odd length */
            int half = (len - 1) / 2;
            seq_t start = pow10[half - 1];
            seq_t end   = pow10[half] - (seq_t)1;

            for (seq_t i = start; i <= end; ++i) {
                seq_t rev = reverse_digits(i);
                for (int m = 0; m <= 9; ++m) {
                    seq_t val = i * pow10[half + 1] +
                                (seq_t)m * pow10[half] + rev;
                    if (val % (seq_t)8 == 0) {
                        ++count;
                        if (count == n)
                            return val;
                    }
                }
            }
        }
    }

    /* should never reach here for valid inputs */
    return (seq_t)0;
}
