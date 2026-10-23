/*
 * A170782  a(n) = n^8*(n^10 + 1)/2.
 * https://oeis.org/A170782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^10 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to appropriate sequence type */
    seq_t n_t = (seq_t)n;

    /* Compute powers with a minimal number of operations */
    seq_t n2  = n_t * n_t;          /* n^2   */
    seq_t n4  = n2  * n2;           /* n^4   */
    seq_t n8  = n4  * n4;           /* n^8   */
    seq_t n10 = n8  * n2;           /* n^10  */

    /* Avoid overflow by dividing by 2 as early as possible.
       The product n^8 * (n^10 + 1) is always even. */
    if ((n & 1) == 0) {           /* n is even  -> n^8 is even   */
        seq_t n8_half = n8 / 2;
        return n8_half * (n10 + 1);
    } else {                      /* n is odd   -> n^10 + 1 is even */
        seq_t n10_half = (n10 + 1) / 2;
        return n8 * n10_half;
    }
}
