/*
 * A062730  Rows of Pascal's triangle which contain 3 terms in arithmetic
 *          progression.
 * https://oeis.org/A062730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   for n>4, a(n) = (n^2+8n+8)/4 if n is even, (n^2+10n+9)/4 if n is
 *            odd
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base cases: a(1)=7, a(2)=12, a(3)=14, a(4)=19. */
seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)7;
        case 2: return (seq_t)12;
        case 3: return (seq_t)14;
        case 4: return (seq_t)19;
    }
    if ((n & (idx_t)1) == 0) {                /* even n */
        seq_t val = (seq_t)n * (seq_t)n;      /* n^2 */
        val += (seq_t)8 * n;
        val += (seq_t)8;
        return val / (seq_t)4;
    } else {                                  /* odd n */
        seq_t val = (seq_t)n * (seq_t)n;      /* n^2 */
        val += (seq_t)10 * n;
        val += (seq_t)9;
        return val / (seq_t)4;
    }
}
