/*
 * A047253  Numbers that are congruent to {1, 2, 3, 4, 5} mod 6.
 * https://oeis.org/A047253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + floor((n+4)/5) (numbers that are congruent to
 *            {1,2,3,4,5} modulo 6)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the target integer type to avoid overflow */
    seq_t t = (seq_t)n;
    /* If n is 0 (outside the intended offset), return 0 to avoid underflow */
    if (t == 0) {
        return 0;
    }
    return t - 1 + (t + 4) / 5;
}
