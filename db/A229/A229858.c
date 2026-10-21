/*
 * A229858  Consider all 120-degree triangles with sides A < B < C. The
 *          sequence gives the values of A.
 * https://oeis.org/A229858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+4 for n>4. Special cases: a(1)=3, a(2)=5, a(3)=6,
 *            a(4)=7
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1u: return (seq_t)3;
        case 2u: return (seq_t)5;
        case 3u: return (seq_t)6;
        case 4u: return (seq_t)7;
        default: return (seq_t)n + (seq_t)4;
    }
}
