/*
 * A081359  Problem 49 on page 235 of Mathematics: California Edition, by
 *          Carole Greenes et al.
 * https://oeis.org/A081359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn
 * Formula:   a(1) = 6 a(n) = 7 + 3*(n-2)*(n-1)/2 for n >= 2
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula follows from the constant‑difference pattern
   1, 3, 6, 9, 12,… in the first differences of the sequence. */

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=6 */
    if (n <= 1)
        return (seq_t)6;

    /* For n >= 2 use the closed form */
    seq_t a_val = (seq_t)7;           /* base value */

    /* Compute 3*(n-2)*(n-1)/2.
       The product (n-2)*(n-1) is even, so division by 2 is exact
       and performed before the multiplication by 3 to avoid overflow
       on 64‑bit seq_t. */
    seq_t p1 = (seq_t)(n - 2);
    seq_t p2 = (seq_t)(n - 1);
    seq_t prod = p1 * p2;
    prod /= (seq_t)2;      /* exact division */
    prod *= (seq_t)3;      /* multiply by 3 */

    a_val += prod;
    return a_val;
}
