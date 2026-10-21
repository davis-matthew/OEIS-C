/*
 * A236313  Recurrence: a(2n) = 3a(n)-1, a(2n+1) = 1.
 * https://oeis.org/A236313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(2n) = 3*a(n) - 1, a(2n+1) = 1
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against invalid 0 index; the sequence is defined for n ≥ 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t val = (seq_t)1;          /* a(1) = 1 */
    while ((n & 1) == 0) {         /* while n is even */
        val = (seq_t)3 * val - (seq_t)1;
        n >>= 1;                   /* n = n / 2 */
    }
    return val;                    /* n is now odd → a(n) = 1, or we have reached the result */
}
