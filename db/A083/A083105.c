/*
 * A083105  Second-order linear recurrence sequence with a(n) = a(n-1) +
 *          a(n-2), with initial terms 62638280004239857, 49463435743205655.
 * https://oeis.org/A083105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 62638280004239857, a(1) =
 *            49463435743205655
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0)
        return (seq_t)62638280004239857ULL;
    if (n == 1)
        return (seq_t)49463435743205655ULL;

    /* Iterate the recurrence */
    seq_t prev = (seq_t)62638280004239857ULL;  /* a(0) */
    seq_t curr = (seq_t)49463435743205655ULL;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}
