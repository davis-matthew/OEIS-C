/*
 * A130233  a(n) is the maximal k such that Fibonacci(k) <= n (the "lower"
 *          Fibonacci Inverse).
 * https://oeis.org/A130233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the largest k such that Fibonacci(k) <= n (i.e. a(n) =
 *            floor(log_phi((sqrt(5)*n + sqrt(5*n^2+4))/2)) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Fibonacci sequence with F(0)=0, F(1)=1.  We iterate until
       the Fibonacci value exceeds n; the previous value is the
       answer.  idx_t is used for the arithmetic; seq_t only for
       the return type. */
    idx_t prev = 0;
    idx_t cur  = 1;
    idx_t idx  = 1;           /* current index of cur */

    /* Special case: n == 0 gives index 0 (F(0)=0).  The loop
       below will return idx-1 = 0 automatically. */
    while (cur <= n) {
        /* compute next Fibonacci number, checking for overflow. */
        idx_t next;
#ifdef UINT64_MAX
        if (prev > UINT64_MAX - cur) {
            /* will overflow, treat as a huge number > n */
            next = UINT64_MAX;
        } else {
            next = prev + cur;
        }
#else
        next = prev + cur;
#endif
        prev = cur;
        cur  = next;
        idx++;
    }

    return (seq_t)(idx - 1);
}
