/*
 * A203170  Sum of the fourth powers of the first n odd-indexed Fibonacci
 *          numbers.
 * https://oeis.org/A203170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1..n} F(2k-1)^4. Computation is performed by
 *            iterating the Fibonacci sequence up to index 2n and summing the
 *            fourth powers of the odd indices. This avoids intermediate
 *            values that would overflow the seq_t type used by the build. The
 *            method is O(n) and works for all defined seq_t and i...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prev = 0;   /* F(k-1) */
    seq_t cur  = 1;   /* F(k)   */

    for (idx_t k = 1; k <= 2 * n; ++k) {
        if (k & 1) {              /* odd index → F(2k-1)^4 */
            seq_t t = cur * cur;  /* F(k)^2 */
            t = t * t;            /* F(k)^4 */
            sum += t;
        }
        /* advance to next Fibonacci number */
        seq_t next = prev + cur;  /* F(k+1) */
        prev = cur;
        cur  = next;
    }
    return sum;
}
