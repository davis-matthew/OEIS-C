/*
 * A215530  The limit of the string "0, 1" under the operation 'repeat string
 *          twice and append 0'.
 * https://oeis.org/A215530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th element of the infinite string obtained by repeatedly
 * replacing S by S S 0 starting from S = "01" (a(0)=0, a(1)=1).
 * Equivalent recursive definition:
 *
 *   let L_k = 3·2^k - 1   (length of S_k)
 *   a(0)=0, a(1)=1
 *   for n>1:
 *     choose k such that L_{k-1} ≤ n < L_k
 *     if n == L_k-1          → a(n)=0  (the appended 0)
 *     else                   → a(n)=a(n - L_{k-1})   (second copy of S_{k-1})
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* work with the index as seq_t */

    /* base cases handled automatically when t <= 1 */
    while (t > 1)
    {
        seq_t l   = (seq_t)2;    /* L_0 */
        seq_t prev = l;          /* L_{k-1} */

        /* find the smallest length L_k that is > t */
        while (l <= t)
        {
            prev = l;
            l = l * (seq_t)2 + (seq_t)1;   /* L_{k} = 2·L_{k-1} + 1 */
        }

        /* if t is the last position of the current level, the value is 0 */
        if (t == l - (seq_t)1)
            return (seq_t)0;

        /* otherwise t lies in the second copy of the previous string,
         * so we subtract the length of that previous string */
        if (t >= prev)
            t -= prev;
        /* if t < prev, it already points into the first copy and
         * we just loop again with the smaller index          */
    }

    /* t is 0 or 1 */
    return t;
}
