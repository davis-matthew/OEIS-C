/*
 * A054890  Layer counting sequence for hyperbolic tessellation by regular
 *          heptagons of angle Pi/3.
 * https://oeis.org/A054890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=7; a(3)=42;
   a(n)=6*a(n-1)-a(n-2) for n>=4   (valid for all n>=1) */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)42;

    /* iterative recurrence for n >= 4 */
    seq_t prev2 = (seq_t)7; /* a(2) */
    seq_t prev1 = (seq_t)42;/* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = 6 * prev1 - prev2; /* a(i) = 6*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
