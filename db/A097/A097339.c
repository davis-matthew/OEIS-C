/*
 * A097339  a(n) = 2^n+n^3.
 * https://oeis.org/A097339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + n^3
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n by fast exponentiation */
    seq_t pow2 = (seq_t)1;
    seq_t base = (seq_t)2;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            pow2 *= base;
        }
        base *= base;
        e >>= 1;
    }

    /* Compute n^3 */
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;

    return pow2 + n3;
}
