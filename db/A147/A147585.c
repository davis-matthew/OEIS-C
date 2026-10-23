/*
 * A147585  a(1) = 1; a(n) = (7*n-9)*a(n-1) for n > 1.
 * https://oeis.org/A147585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*n-9)*a(n-1), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For indices < 1 we return 0. */
    if (n == 0)
        return 0;

    seq_t result = 1;                /* a(1) */

    /* Multiply successively by the recurrence factor (7*i-9). */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)7 * (seq_t)i - (seq_t)9;
        result *= factor;
    }
    return result;
}
