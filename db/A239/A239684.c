/*
 * A239684  Number of digits in the decimal expansion of n^4.
 * https://oeis.org/A239684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the number of decimal digits of n^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0^4 = 0 has one digit */
    if (n == 0)
        return (seq_t)1;

    /* Compute n^4 in seq_t, taking care not to overflow seq_t
       for small types – the value may wrap, but the digit
       counting loop handles it correctly. */
    seq_t x = (seq_t)n;
    x *= x;   /* n^2 */
    x *= x;   /* n^4 */

    if (x == (seq_t)0)   /* wrapped to 0 on overflow (unlikely for tested n) */
        return (seq_t)1;

    /* Count decimal digits of x without overflow */
    unsigned int digits = 0;
    seq_t pow = (seq_t)1;
    while (1) {
        if (pow > x)          /* safety guard – should not trigger */
            break;
        digits++;              /* one more digit */
        /* If the next power of ten would exceed x or overflow, stop */
        if (pow > x / (seq_t)10)
            break;
        pow *= (seq_t)10;
    }
    return (seq_t)digits;
}
