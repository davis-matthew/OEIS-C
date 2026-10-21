/*
 * A099944  Number of 3 X n binary matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1) and (11;0).
 * https://oeis.org/A099944
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*2^n - 12 (valid for n ≥ 3)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset (n < 3) the value is undefined;
       return 0 to keep the function well‑defined for all idx_t. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* Compute 2^n by repeated multiplication.  For unsigned integer
       types this is well‑defined (it wraps modulo 2^width) and for
       arbitrary‑precision seq_t implementations it simply grows as
       needed.  The loop runs n times; for n up to 10,000 this is
       comfortably fast (≈10⁴ mults). */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* a(n) = 11 * 2^n - 12 */
    return (seq_t)11 * pow2 - (seq_t)12;
}
