/*
 * A130245  Number of Lucas numbers (A000032) <= n.
 * https://oeis.org/A130245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Lucas numbers (A000032) <= n Compute by simple
 *            iteration: 2, 1, and then Lucas numbers generated as a+b until
 *            the next exceeds n. No floating point and works for any unsigned
 *            integer type defined as seq_t
 * Range:     u8 n <= 944, u16 n <= 9030, u32 n <= 9030, u64 n <= 9030,
 *            u128 n <= 9030, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s_n = (seq_t)n;              /* convert index to seq_t for comparison */
    seq_t a0 = (seq_t)2;               /* Lucas(0) */
    seq_t a1 = (seq_t)1;               /* Lucas(1) */

    /* Count the trivial cases first */
    seq_t count;
    if (s_n >= (seq_t)2)
        count = (seq_t)2;              /* Lucas(0)=2 and Lucas(1)=1 are <= n */
    else if (s_n == (seq_t)1)
        count = (seq_t)1;              /* only Lucas(1)=1 */
    else
        return (seq_t)0;               /* n < 1 -> no Lucas numbers */

    if (s_n >= (seq_t)2) {
        /* Generate Lucas numbers starting from index 2 */
        seq_t prev = a0;   /* L(k-2) */
        seq_t curr = a1;   /* L(k-1) */
        while (1) {
            seq_t next = prev + curr;          /* L(k) = L(k-1) + L(k-2) */
            if (next < prev)                   /* overflow detection for uint64_t */
                break;                         /* beyond the largest representable value */

            if (next > s_n)                    /* stop when exceeding n */
                break;

            count++;                           /* next is <= n, include it */

            /* Shift for next iteration: L(k-1) becomes prev, L(k) becomes curr */
            prev = curr;
            curr = next;
        }
    }

    return count;
}
