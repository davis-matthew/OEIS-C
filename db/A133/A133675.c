/*
 * A133675  Negative discriminants with form class number 1 (negated).
 * https://oeis.org/A133675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th positive integer d such that d ≡ 0 or 1 (mod 4)
 * and the imaginary quadratic field 𝔔(√−d) has class number 1.
 * For n > 13 the sequence terminates and a(n) = 0. */

#include <stddef.h>   /* for size_t */

static const seq_t _a_values[] = {
    (seq_t)3,
    (seq_t)4,
    (seq_t)7,
    (seq_t)8,
    (seq_t)11,
    (seq_t)12,
    (seq_t)16,
    (seq_t)19,
    (seq_t)27,
    (seq_t)28,
    (seq_t)43,
    (seq_t)67,
    (seq_t)163
};

static const size_t _a_count = sizeof(_a_values) / sizeof(_a_values[0]);

seq_t a(idx_t n)
{
    if (n == 0 || n > _a_count)
        return (seq_t)0;
    return _a_values[(size_t)(n - 1)];
}
