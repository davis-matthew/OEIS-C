/*
 * A028160  Expansion of 1/((1-4x)(1-8x)(1-11x)(1-12x)).
 * https://oeis.org/A028160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 132*a(n-2) + 2^(3n+1) - 4^n with a(0) = 1,
 *            a(1) = 35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)35;

    seq_t a0 = (seq_t)1;   /* a(n-2)   */
    seq_t a1 = (seq_t)35;  /* a(n-1)   */
    seq_t a2;              /* a(n)     */

    /* Start powers for i = 1:
       pow2 = 2^(3*1+1) = 2^4 = 16
       pow4 = 4^1 = 4   */
    seq_t pow2 = (seq_t)16;
    seq_t pow4 = (seq_t)4;

    for (idx_t i = 2; i <= n; ++i) {
        /* Increment the exponents for the new i:
           2^(3i+1) = 2^(3(i-1)+1) * 2^3
           4^i      = 4^(i-1)      * 4          */
        pow2 *= (seq_t)8;
        pow4 *= (seq_t)4;

        a2 = (seq_t)23 * a1 - (seq_t)132 * a0 + pow2 - pow4;

        a0 = a1;
        a1 = a2;
    }
    return a2;
}
