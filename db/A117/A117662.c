/*
 * A117662  a(n) = n*(n-1)*(n-2)*(n+3)/12.
 * https://oeis.org/A117662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+3)/12
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 317, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the product contains zero, and the result is zero. */
    if (n < 3) return 0;

    seq_t tn = (seq_t)n;
    /* All factors are positive for n >= 3, so unsigned arithmetic is safe. */
    seq_t prod = tn * (tn - 1);
    prod *= (tn - 2);
    prod *= (tn + 3);
    return prod / 12;
}
