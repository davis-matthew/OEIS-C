/*
 * A178965  a(n) = numerator of Sum_{k>=1} floor(n/k)/2^k.
 * https://oeis.org/A178965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Sum_{i=1..n} 2^(n-i)*floor(n/i)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return 0;

    /* Use seq_t for all arithmetic; the compiler and build may replace
       seq_t by uint8_t, uint16_t, uint32_t, uint64_t, __uint128_t, or an
       arbitrary‑precision class.  The implementation therefore works
       uniformly on all available integer sizes.      */
    seq_t total = 0;
    seq_t pow2   = 1;           /* 2^0 */

    /* Iterate i from n down to 1, accumulating the term 2^(n-i)*floor(n/i)
       and doubling pow2 each step.                            */
    for (idx_t i = n; i > 0; --i) {
        seq_t q = (seq_t)(n / i);    /* floor(n/i) */
        total += pow2 * q;
        pow2 <<= 1;                  /* multiply by 2 for next term */
    }
    return total;
}
