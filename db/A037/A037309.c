/*
 * A037309  Numbers whose base-3 and base-4 expansions have the same digit
 *          sum.
 * https://oeis.org/A037309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑3 digit sum equals its base‑4 digit sum. */

#include <stdbool.h>

/* Helper: return sum of digits of x in the given base. */
static unsigned digit_sum_base(seq_t x, seq_t base)
{
    unsigned sum = 0;
    const seq_t zero = (seq_t)0;
    while (x > zero) {
        sum += (unsigned)(x % base);
        x /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n = 0; return 0 for safety */
        return 0;

    seq_t num = (seq_t)0;
    idx_t count = 0;
    const seq_t B3 = (seq_t)3;
    const seq_t B4 = (seq_t)4;

    while (count < n) {
        ++num;                       /* candidate number */
        unsigned s3 = digit_sum_base(num, B3);
        unsigned s4 = digit_sum_base(num, B4);
        if (s3 == s4)
            ++count;
    }

    return num;
}
