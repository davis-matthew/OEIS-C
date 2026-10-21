/*
 * A204078  Number of nX2 0..3 arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A204078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * round(2^n ÷ 7)^2 Rewritten with integer arithmetic:
 *            round(2^n ÷ 7) = floor( (2^(n+1) + 7) ÷ 14 ) So a(n) = 4 * ((
 *            (2^(n+1) + 7) ÷ 14 )^2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Underflow safe: for n < 1 return 0 --- undefined but harmless */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(n+1) in a type‑safe way (works for all seq_t types) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n + (idx_t)1; ++i)
        pow2 *= (seq_t)2;

    /* nearest integer to 2^n/7 : floor( (pow2 + 7) / 14 ) */
    seq_t t = (pow2 + (seq_t)7) / (seq_t)14;

    /* a(n) = 4 * t^2 */
    return (seq_t)4 * t * t;
}
