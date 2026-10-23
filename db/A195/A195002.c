/*
 * A195002  Number of lower triangles of a 6 X 6 0..n array with each element
 *          differing from all of its horizontal and vertical neighbors by
 *          one.
 * https://oeis.org/A195002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61488*n - 169100 for n>8, otherwise a(1)=2, a(2)=4608,
 *            a(3)=31042, a(4)=81014, a(5)=138956, a(6)=199988, a(7)=261324,
 *            a(8)=322806
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 8 terms (index 1..8).  element 0 unused. */
    static const seq_t first[9] = {
        0,          /* dummy for index 0 */
        2,          /* a(1) */
        4608,       /* a(2) */
        31042,      /* a(3) */
        81014,      /* a(4) */
        138956,     /* a(5) */
        199988,     /* a(6) */
        261324,     /* a(7) */
        322806      /* a(8) */
    };

    if (n <= 8) {
        /* guard against n == 0 (offset not defined) */
        return (n >= 1) ? first[n] : 0;
    }

    /* 61488*n - 169100  (n >= 9).  Use 64‑bit intermediate to avoid overflow
       before the final cast to seq_t.  For small seq_t the cast will
       automatically wrap modulo the appropriate width. */
    return (seq_t)(61488ULL * (unsigned long long)n - 169100ULL);
}
