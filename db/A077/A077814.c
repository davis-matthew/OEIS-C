/*
 * A077814  a(n) = #{0<=k<=n: mod(k*n,4)=2}.
 * https://oeis.org/A077814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{0 <= k <= n : mod(k*n, 4) = 2} Piecewise closed form:
 *            if n = 4k => a(n) = 0 if n = 4k + 1 => a(n) = k if n = 4k + 2 =>
 *            a(n) = 2k + 1 if n = 4k + 3 => a(n) = k + 1 This implementation
 *            uses only idx_t arithmetic and then casts the result to seq_t,
 *            avoiding any operations that might fail on...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 4;          /* floor(n/4) */
    idx_t r = n % 4;          /* n mod 4, 0..3 */

    switch (r) {
        case 0: return (seq_t)0;                   /* n = 4k      */
        case 1: return (seq_t)q;                   /* n = 4k + 1  */
        case 2: return (seq_t)(2 * q + 1);         /* n = 4k + 2  */
        case 3: return (seq_t)(q + 1);             /* n = 4k + 3  */
    }
    /* Unreachable but suppress compiler warnings */
    return (seq_t)0;
}
