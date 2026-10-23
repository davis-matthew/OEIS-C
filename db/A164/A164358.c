/*
 * A164358  Expansion of (1 - x^2)^2 * (1 - x^3) / ((1 - x)^3 * (1 - x^4)) in
 *          powers of x.
 * https://oeis.org/A164358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n) repeats 3,4,3,2 for n%4=1,2,3,0
 *            respectively. Sequence: 1,3,4,3,2,3,4,3,2,
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Determine n modulo 4 using bitwise AND (since 4 is 2^2). */
    switch ((int)(n & 3))
    {
        case 1:  return (seq_t)3;  /* n = 4k+1  → 3 */
        case 2:  return (seq_t)4;  /* n = 4k+2  → 4 */
        case 3:  return (seq_t)3;  /* n = 4k+3  → 3 */
        case 0:  return (seq_t)2;  /* n = 4k    → 2 */
        default: return (seq_t)0;  /* unreachable */
    }
}
