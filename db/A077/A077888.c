/*
 * A077888  Expansion of (1-x)^(-1)/(1+x^2-x^3).
 * https://oeis.org/A077888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 57, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077888: 
   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - a(n-4)   (n ≥ 4)
   with initial values a(0)=1, a(1)=1, a(2)=0, a(3)=1.
   This follows from 1/((1-x)(1+x²-x³)) = Σ a(n)xⁿ. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)0;   /* a(n-2) */
    seq_t a3 = (seq_t)1;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 - a2 + (a1 << 1) - a0;  /* a(n) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
