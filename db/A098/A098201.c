/*
 * A098201  Number of distinct terms in iteration-list when
 *          cototient-function[=A051953] is iterated and the initial value is
 *          odd number.
 * https://oeis.org/A098201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct terms in the sequence obtained by
 *            iterating the cototient function n -> n - φ(n) starting from the
 *            odd integer (2n-1) until 0 is reached. The count includes the
 *            initial odd number and the final 0. This equals A053475(2n-1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(m) for an arbitrary-precision or fixed-size seq_t value.  */
static seq_t phi_seq(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;
    /* Run trial division.  For small m this is fast. */
    for (; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                m = m / p;
            }
            result = result / p * (p - 1);
        }
    }
    if (m > 1) {
        result = result / m * (m - 1);
    }
    return result;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Start value is the odd integer 2n-1. */
    seq_t cur = (seq_t)n * 2 - 1;
    seq_t count = 1;          /* count the starting value itself */

    while (cur != 0) {
        seq_t phi_val = phi_seq(cur);
        cur = cur - phi_val;  /* next term in the iteration */
        ++count;
    }

    return count;
}
