/*
 * A092398  Row 4 of array in A288580.
 * https://oeis.org/A092398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{i = 0 .. ⌊n/2⌋, n-4i ≠ 0} (n-4i)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the OEIS description: a(n) = !n!_4 = Prod_{i} (n-4i),   */
/* where the product runs over all i with 0 < |n-4i| ≤ n and n-4i ≠ 0.  */

seq_t a(idx_t n)
{
    seq_t prod = 1;                     /* product of all required factors   */
    idx_t max_i = n / 2;                /* largest i with |n-4i| ≤ n         */
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t val = (seq_t)n - (seq_t)(4 * i);   /* n - 4*i  */
        if (val == 0)                    /* skip the zero term (only possible when n % 4 == 0) */
            continue;
        prod *= val;                     /* multiply into the running product    */
    }
    return prod;
}
