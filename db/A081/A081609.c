/*
 * A081609  Number of numbers <= n having at least one 1 in their ternary
 *          representation.
 * https://oeis.org/A081609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) - f(n), where f(n) = # of integers 0 ≤ x ≤ n whose
 *            ternary representation uses only the digits 0 and 2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Count numbers 0..n whose ternary representation uses only 0 and 2. */
static seq_t count_no1(idx_t n)
{
    /* Convert n to base-3 digits, most significant first. */
    idx_t digits[65];
    size_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = tmp % 3;
        tmp /= 3;
    } while (tmp);

    /* Reverse to MSB→LSB order. */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        idx_t t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }

    seq_t tight = 1; /* number of ways that stayed equal to prefix of n */
    seq_t loose = 0; /* number of ways that are already smaller */

    for (size_t pos = 0; pos < len; ++pos) {
        idx_t d_n = digits[pos];
        seq_t next_tight = 0;
        seq_t next_loose = 0;

        /* Allowed digits are 0 and 2. */
        for (idx_t d = 0; d <= 2; d += 2) {
            if (d <= d_n) {
                if (d == d_n)
                    next_tight += tight;
                else
                    next_loose += tight;
            }
        }
        /* From loose state, both digits keep it loose. */
        next_loose += loose * 2;

        tight = next_tight;
        loose = next_loose;
    }

    return tight + loose;
}

seq_t a(idx_t n)
{
    /* Count all numbers 0..n. */
    seq_t total = (seq_t)n + (seq_t)1;

    /* Subtract those without a digit 1 in base 3. */
    seq_t without_one = count_no1(n);
    return total - without_one;
}
