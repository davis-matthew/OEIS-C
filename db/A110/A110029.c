/*
 * A110029  a(n) is the minimal c >= n such that the sum of consecutive
 *          integers from n to c is a Niven number.
 * https://oeis.org/A110029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal c ≥ n such that Σ_{k=n}^c k is a Niven number
 * Range:     u8 n <= 51, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute digit sum of a seq_t value. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but safe */

    seq_t c = (seq_t)n;                    // current upper bound
    seq_t sum = c;                         // sum from n to c

    while (1) {
        seq_t sumbd = digit_sum(sum);
        if (sum % sumbd == 0) return c;   // Niven found

        ++c;                               // extend the range
        sum += c;
    }
}
