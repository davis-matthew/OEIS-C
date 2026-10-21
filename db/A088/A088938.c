/*
 * A088938  Occurrences of 2's in A088936.
 * https://oeis.org/A088938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + (1/2) * sum_{k=1}^{n-1} sum_{i=0}^{k} i!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Except for n=1 (offset), the implementation avoids nested loops 
   by re‑ordering the sums:
   a(n) = 2 + (1/2) * [ (n-1)*0! + Σ_{i=1}^{n-1} (n-i)*i! ]   */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence (offset 1) */
    if (n <= 1)
        return (seq_t)2;

    seq_t sum = 0;
    seq_t f   = 1;                 /* factorial i!   (i = 0) */

    /* weight for i = 0 is n-1 */
    sum += ((seq_t)(n - 1)) * f;

    /* loop for i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i)
    {
        f *= (seq_t)i;              /* f = i!          */
        sum += ((seq_t)(n - i)) * f;/* add (n-i) * i!  */
    }

    /* a(n) = 2 + sum/2  */
    return ((seq_t)2) + (sum / 2);
}
