/*
 * A132223  A dense infinitive sequence.
 * https://oeis.org/A132223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A132223 (dense infinitive sequence) by
   iterative decomposition of the segment containing the desired
   index.  The sequence is defined recursively as follows:

   For k ≥ 2, let len = 2^k and let a_k[0..len-1] be the segment of
   length len.  Then

      a_k[2*i]   = a_{k-1}[i]                     (0 ≤ i < len/2)
      a_k[2*i+1] = len - i                       (0 ≤ i < len/2)

   The overall sequence is the concatenation of the segments
   a_1, a_2, a_3, …   (a_1 = [1,2]).

   This function finds the segment containing n, computes the
   offset within that segment and evaluates a_k[offset] using the
   above recurrence.  The algorithm runs in O(log n) time. */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t t   = n + 1;                         /* ≥ 2 */
    /* floor(log2(t)) */
    idx_t k   = 63 - __builtin_clzll(t);
    idx_t len = ((idx_t)1) << k;              /* 2^k  */
    idx_t start = len - 1;                    /* index of first element of this segment */
    idx_t r = n - start;                      /* offset inside the segment */

    while (len > 2) {
        if (r & 1)                            /* odd offset → new number */
            return (seq_t)len - (seq_t)(r >> 1);
        else {                                /* even offset → recurse on shorter segment */
            r >>= 1;
            len >>= 1;
        }
    }
    /* len == 2: base case a_1 = [1,2] */
    return r == 0 ? (seq_t)1 : (seq_t)2;
}
