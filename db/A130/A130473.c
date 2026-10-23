/*
 * A130473  Partial sums of A087172.
 * https://oeis.org/A130473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(Fib(2*b(n)+1) - 3*Fib(b(n))*Fib(b(n)+1)-1) +
 *            (n+1)*Fib(b(n)) * where b(n) is the segment index: the Fibonacci
 *            number which occurs * as the forward difference a(n)-a(n-1). The
 *            sequence of differences is * the Fibonacci numbers F_k repeated
 *            F_{k-1} times (with F_0=0, F_1=1). * The imp...
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1 */
    if (n == 1)
        return (seq_t)1;

    /* Current accumulated value a(i) for i = next_index-1 */
    seq_t acc = (seq_t)1;

    /* next_index is the first index in the current segment */
    idx_t next_index = 2;

    /* The slope of the current segment is Fib(t),
     * where the length of the segment is Fib(t-1).
     * The first slope that actually appears in the sequence is Fib(3)=2,
     * so we start with t = 3. */
    idx_t t = 3;

    while (1) {
        /* Length of the current segment */
        idx_t len = (idx_t)fib(t - 1);

        /* Skip the (non‑existent) t = 2 segment, which would have length 0 */
        if (len == 0) {
            ++t;
            continue;
        }

        /* The last index covered by this segment */
        idx_t end = next_index + len - 1;

        if (n <= end) {
            /* n lies inside this segment */
            idx_t steps = n - next_index + 1;          /* steps from the segment start */
            seq_t slope = fib(t);                      /* Fib(t) */
            acc += (seq_t)steps * slope;               /* accumulate the partial sum */
            return acc;
        }

        /* n is after the entire segment:
         * add the whole contribution of this segment
         * and move to the next one. */
        acc += (seq_t)len * fib(t);
        next_index = end + 1;
        ++t;
    }
}
