/*
 * A092442  Sequence arising from enumeration of domino tilings of Aztec
 *          Pillow-like regions.
 * https://oeis.org/A092442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(2^n-1) - n^2 (offset 0)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n+1 to seq_t */
    seq_t x = (seq_t)n + 1;

    /* compute 2^n in seq_t */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
    }

    /* compute (n+1)*(2^n-1) - n^2 */
    seq_t val = x * pow2;   /* (n+1)*2^n  */
    val -= x;               /* subtract (n+1) -> (n+1)*(2^n-1) */
    seq_t nsq = (seq_t)n * (seq_t)n;
    val -= nsq;             /* final formula */

    return val;
}
