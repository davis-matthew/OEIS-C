/*
 * A222472  Numerator sequence of the n-th convergent of the continued
 *          fraction 1/(1+3/(2+3/(3+3/(4+...
 * https://oeis.org/A222472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + 3*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n>=1 but safe fallback */
    if (n == 1) return 1;

    seq_t a0 = 0;  /* a(0) */
    seq_t a1 = 1;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = (seq_t)i * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
