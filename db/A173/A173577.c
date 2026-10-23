/*
 * A173577  7-Factorions: equal to the sum of the 7-fold factorials of their
 *          digits in base 10.
 * https://oeis.org/A173577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   a(n) = the nth positive integer that equals the sum of the
 *            septuple factorials of its decimal digits. The septuple
 *            factorial of a digit d = d · (d-7) · (d-14) · … until the factor
 *            becomes ≤0. For the digits 0‑9 this reduces to the values in the
 *            table below. The sequence is finite; the first ten t...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t septuple_factorial[10] = {
    1ULL,    /* 0 */
    1ULL,    /* 1 */
    2ULL,    /* 2 */
    3ULL,    /* 3 */
    4ULL,    /* 4 */
    5ULL,    /* 5 */
    6ULL,    /* 6 */
    7ULL,    /* 7 */
    8ULL,    /* 8 */
    18ULL    /* 9 = 9·2 */
};

static seq_t septuple_sum_of_digits(seq_t x)
{
    seq_t sum = 0;
    if (x == 0) return septuple_factorial[0];
    while (x != 0) {
        unsigned d = (unsigned)(x % 10ULL);
        sum += septuple_factorial[d];
        x /= 10ULL;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    seq_t m = 1;
    /* The sequence is very short.  We search up to a generous upper bound
       to avoid an infinite loop if the caller requests an index beyond the
       actual length. */
    for (; m <= 2000ULL; ++m) {
        if (m == septuple_sum_of_digits(m)) {
            ++found;
            if (found == n) return m;
        }
    }
    return 0; /* not found within search range */
}
