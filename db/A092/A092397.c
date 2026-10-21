/*
 * A092397  Row 3 of array in A288580.
 * https://oeis.org/A092397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*** 
 *  A092397: double-factorial-like product with step 3.
 *  
 *  For a non‑negative integer n,
 *   a(n) = ∏  (n - 3⋅i)   over all i ≥ 0 such that  
 *          0 < |n - 3⋅i| ≤ n  and  (n - 3⋅i) ≠ 0.
 *  The product is over the numbers n, n–3, n–6, …, n–3⋅⌊(n−1)/3⌋,
 *  followed by the negative terms −3, −6, … as long as their absolute
 *  value does not exceed n.
 *
 *  Implementation: iteratively multiply the appropriate terms.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty product). */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* i starts at 0; we continue until |n-3i| > n. */
    for (idx_t i = 0; ; ++i)
    {
        seq_t term = (seq_t)n - (seq_t)3 * (seq_t)i;
        if (term == 0)
            continue;                    /* skip the zero factor */
        seq_t abs_term = term >= 0 ? term : -term;
        if (abs_term > (seq_t)n)        /* outside the allowed range */
            break;
        result *= term;                 /* multiply the term */
    }
    return result;
}
