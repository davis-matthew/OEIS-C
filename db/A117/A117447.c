/*
 * A117447  Expansion of g.f. (1 + 2*x + 3*x^2 + x^3)/(1 + x - x^3 - x^4).
 * https://oeis.org/A117447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3 mod 2) + ((n+3)^2 mod 3) for all n≠2, a(0)=1,
 *            a(1)=1, a(2)=2. This reproduces the periodic pattern
 *            1,1,2,0,2,1,1,1,2,0,2,1,1,1,2,
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    idx_t m = n + 3;                     /* n+3 */
    unsigned mod2 = (unsigned)(m & 1);   /* (n+3) mod 2 */
    unsigned r   = (unsigned)(m % 3);    /* (n+3) mod 3 */
    unsigned mod3 = (unsigned)((r * r) % 3); /* ((n+3)^2) mod 3 */
    return (seq_t)mod2 + (seq_t)mod3;
}
