/*
 * A073047  Least k such that x(k)=0 where x(1)=n and x(k)=k*floor(x(k-1)/k).
 * https://oeis.org/A073047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2427, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A073047
   Definition: a(n) = least k such that
                x(1)=n,
                x(k) = k * floor(x(k-1)/k)
   The sequence terminates when the previous value becomes smaller than the
   new k, because then floor(...) = 0 and x(k)=0.
   The algorithm iteratively reduces the current value by the remainder
   modulo k until that condition is met.  It runs in O(a(n)) steps,
   which is O(√n) in practice. */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;   /* current value of the sequence */
    for (idx_t k = 1; ; ++k) {
        seq_t kval = (seq_t)k;
        if (cur < kval)          /* next iteration would be zero */
            return (seq_t)k;
        /* reduce cur to the largest multiple of k <= cur */
        cur = (cur / kval) * kval;
    }
}
