/*
 * A060782  Degree of the curve C(n) relative to a triangle ABC with side
 *          lengths a, b and c, given by (x^n- b^n)(y^n-c^n)(z^n-a^n) =
 *          (x^n-c^n)(y^n-a^n)(z^n-b^n) where x, y and z denote the distances
 *          from the variable point to vertices A, B and C respectively.
 * https://oeis.org/A060782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 16n-2 if n is odd, a(n) = 2n-1 if n is even
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if ((n & 1) == 0) {          /* even n */
        return nseq * 2 - 1;
    } else {                      /* odd n */
        return nseq * 16 - 2;
    }
}
