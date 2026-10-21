/*
 * A214972  a(n) = a(floor(2*(n-1)/3)) + 1, where a(0) = 0.
 * https://oeis.org/A214972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(2*(n-1)/3)) + 1, a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n > 0) {
        /* compute floor(2*(n-1)/3) without overflow */
        idx_t temp = n - 1;
        idx_t q = temp / 3;
        idx_t r = temp % 3;
        n = q * 2 + (r * 2) / 3;
        steps++;            /* each step corresponds to +1 in the recurrence */
    }
    return steps;
}
