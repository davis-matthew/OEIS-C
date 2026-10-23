/*
 * A166820  Number of nX3 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*P(n)/181440 where P(n) = 29*n^7 + 569*n^6 +
 *            9815*n^5 + 65759*n^4 + 269228*n^3 + 303248*n^2 - 321048*n +
 *            495648
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A166820
   (offset 1).  The function is safe for arbitrary
   precision types used for seq_t.  No floating-point
   operations are performed.  It is O(1) in time and
   memory. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(1)=0, a(0) unused */

    seq_t ns = (seq_t)n;

    /* Compute P(n) by Horner's rule, accounting for the
       sign of the linear term.  All intermediate results
       fit within seq_t for n up to 10 000 on typical 64‑bit
       arbitrary‑precision builds. */
    seq_t poly = (seq_t)29;
    poly = poly * ns + (seq_t)569;
    poly = poly * ns + (seq_t)9815;
    poly = poly * ns + (seq_t)65759;
    poly = poly * ns + (seq_t)269228;
    poly = poly * ns + (seq_t)303248;
    poly = poly * ns - (seq_t)321048;   /* linear term is negative */
    poly = poly * ns + (seq_t)495648;   /* constant term */

    /* Multiply by n*(n-1) */
    seq_t term = ns * (ns - (seq_t)1);
    seq_t numerator = term * poly;

    /* Divide by 181440 (9! / 2) */
    const seq_t denom = (seq_t)181440;
    return numerator / denom;
}
