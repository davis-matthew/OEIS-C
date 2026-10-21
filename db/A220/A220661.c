/*
 * A220661  Irregular table, where the n-th row consists of numbers 1..n!
 * https://oeis.org/A220661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n - Σ_{i=1}^{k-1} i! where k is the smallest integer *
 *            such that Σ_{i=1}^k i! ≥ n. This gives the value at position n
 *            in the * infinite concatenation of rows 1!, 2!, 3!, ... of the
 *            integers 1..i!
 * Range:     u8 n <= 408, u16 n <= 5913, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* factorial of the current row k (k!); start with 1! for k = 1 */
    idx_t factorial = 1;
    /* cumulative sum of row lengths of all previous rows (Σ_{i=1}^{k-1} i!) */
    idx_t sum = 0;

    for (idx_t k = 1; ; ++k)
    {
        /* Does the desired index n lie in the current row k ? */
        if (n <= sum + factorial)
            return (seq_t)(n - sum);   /* 1‑based offset within the row */

        /* Move to the next row */
        sum += factorial;

        /* Compute the factorial for the next row ( (k+1)! ).
         * For n up to 2⁶⁴−1 the first ~20 factorials fit in idx_t,
         * so we limit the loop to that range. */
        if (k < 20)
            factorial *= (k + 1);
        else
            break; /* safety; should never happen for the allowed range */
    }

    /* In normal use the loop above always returns.
     * Return 0 in the unlikely event of an overflow. */
    return 0;
}
