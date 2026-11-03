/*
 * A106409  n XOR (greatest proper divisor of n).
 * https://oeis.org/A106409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n XOR (greatest proper divisor of n)
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 87 terms through n = 8979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1) return (seq_t)0;

    /* Find smallest prime factor (spf) of n */
    idx_t spf = n;
    if ((n & 1) == 0) {
        spf = 2;
    } else {
        for (idx_t i = 3; i <= n / i; i += 2) {
            if (n % i == 0) {
                spf = i;
                break;
            }
        }
    }

    /* Greatest proper divisor = n / spf */
    seq_t d = (seq_t)(n / spf);

    /* Return n XOR d */
    return (seq_t)n ^ d;
}
