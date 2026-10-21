/*
 * A075258  Larger terms of the pairs (a < b) in the sequence {a,b}->
 *          {Max[{a,b}]-Min[{a,b}],k*Min[{a,b}]} with k=3 and the first pair
 *          {a=1,b=2}. See A075256.
 * https://oeis.org/A075258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = larger element of the nth pair in the sequence generated
 *            by repeatedly applying {a,b} -> sort({max(a,b)-min(a,b),
 *            3*min(a,b)}) starting from the pair (1,2) (offset 1)
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 98, u64 n <= 201,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iteratively generates the pairs until the requested
   index, returning the larger element of the n‑th pair. This runs
   in O(n) time and uses constant additional memory. */
seq_t a(idx_t n)
{
    seq_t min = 1;           /* smaller element of current pair   */
    seq_t max = 2;           /* larger element of current pair    */

    /* iterate through pairs up to n */
    for (idx_t i = 1; i <= n; ++i) {
        /* the current larger term is 'max' */
        if (i == n)
            return max;

        /* compute the next pair */
        seq_t diff = max - min;          /* |a-b|  (since min <= max)   */
        seq_t km   = (min + min) + min;  /* 3 * min (avoids overflow bugs) */

        /* sort the new pair: ensure min <= max */
        if (diff < km) {
            min = diff;
            max = km;
        } else {
            min = km;
            max = diff;
        }
    }

    /* this point is never reached for n >= 1 */
    return 0;
}
