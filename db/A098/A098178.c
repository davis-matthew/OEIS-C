/*
 * A098178  Expansion of (1+x)(1-x+x^2)/((1-x)(1+x^2)).
 * https://oeis.org/A098178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(0) = 1; for n >= 1, a(n) = 1,0,1,2 repeating (period 4)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* special first term */

    /* for n >= 1, repeat sequence 1,0,1,2 */
    switch ((unsigned)n % 4)
    {
        case 1: return 1;        /* n ≡ 1 (mod 4) */
        case 2: return 0;        /* n ≡ 2 (mod 4) */
        case 3: return 1;        /* n ≡ 3 (mod 4) */
        default: return 2;       /* n ≡ 0 (mod 4) */
    }
}
