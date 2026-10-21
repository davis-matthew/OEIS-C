/*
 * A070635  a(n) = n mod (sum of digits of n).
 * https://oeis.org/A070635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n mod (sum of digits of n)
 * Range:     u8 n <= 1546, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1 using only integer operations. */
static inline seq_t digit_sum(idx_t value)
{
    seq_t s = 0;
    while (value > 0) {
        s += (seq_t)(value % 10);
        value /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* Calculate sum of decimal digits of n. */
    seq_t sum = digit_sum(n);
    /* n is guaranteed > 0, so sum > 0. */
    return (seq_t)n % sum;
}
