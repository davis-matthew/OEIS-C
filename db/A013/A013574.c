/*
 * A013574  Minimal scope of an (n,2) difference triangle.
 * https://oeis.org/A013574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n if n mod 4 ∈ {0,1}; a(n) = 3*n+1 if n mod 4 ∈ {2,3}
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* safety; the sequence is defined for n ≥ 1 */
        return 0;
    seq_t tn = (seq_t)n; /* convert index to seq_t before any arithmetic */
    idx_t r = n & 3;     /* n mod 4, using bitwise AND which is well defined for unsigned */
    if (r == 0u || r == 1u)
        return tn * (seq_t)3;
    else
        return tn * (seq_t)3 + (seq_t)1;
}
