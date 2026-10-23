/*
 * A155590  a(n) = 7^n + 2^n - 1.
 * https://oeis.org/A155590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7^n + 2^n - 1
 * Offset: 0
 * Recurrence: a(n) = 9*a(n-1)-14*a(n-2)-6,  a(0)=1, a(1)=8
 *
 * Implementation: iteratively compute 7^n and 2^n using only +, *, and -=
 *   (no floating point, no math library functions).
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    seq_t pow7 = 1;
    for (idx_t i = 1; i <= n; ++i) {
        pow2 *= (seq_t)2;
        pow7 *= (seq_t)7;
    }
    return pow7 + pow2 - (seq_t)1;
}
