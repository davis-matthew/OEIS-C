/*
 * A147597  a(n) is the number whose binary representation is A138146(n).
 * https://oeis.org/A147597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1, a(2)=7, a(3)=31; for n≥4, a(n) = 7*4^(n‑2) + 7
 *            (closed‐form, same as the recurrence a(n)=5*a(n‑1)−4*a(n‑2) for
 *            n>5)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)7;
        case 3: return (seq_t)31;
        default: {
            seq_t pow4 = (seq_t)1;
            idx_t limit = n - (idx_t)2;
            for (idx_t i = 0; i < limit; ++i)
                pow4 *= (seq_t)4;          /* pow4 = 4^(n-2)  */
            return pow4 * (seq_t)7 + (seq_t)7;
        }
    }
}
