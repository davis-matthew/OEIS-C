/*
 * A059171  Size of largest conjugacy class in S_n, the symmetric group on n
 *          symbols.
 * https://oeis.org/A059171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n-2)! (a(1) = a(2) = 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;   /* a(1) = a(2) = 1 */

    /* Compute n!/(n-1) without intermediate overflow.
       Skip multiplying by (n-1) to arrive directly at the desired
       result while staying within the limits of seq_t. */
    seq_t result = (seq_t)1;
    idx_t skip = n - 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (i == skip)
            continue;           /* omit factor (n-1) */
        result *= (seq_t)i;
    }
    return result;
}
