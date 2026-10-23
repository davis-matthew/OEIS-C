/*
 * A193304  Squarefree numbers multiplied by powers of 5.
 * https://oeis.org/A193304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A193304.
   A(n) is the sequence of positive integers that
   have no square factor other than possibly 5^2, i.e.
   after removing all powers of 5 the remaining part
   is squarefree. This implementation checks integers
   in increasing order until the desired count is reached. */

#include <stdbool.h>

/* Helper: returns true if x is admissible, i.e.
   x / 5^v5(x) has no repeated prime factor. */
static bool is_allowed(seq_t x)
{
    /* Strip all factors of 5. */
    while (x % 5 == 0)
        x /= 5;

    /* Test that the remaining part is squarefree. */
    for (seq_t p = 2; p * p <= x; ++p) {
        if (x % (p * p) == 0)
            return false;
    }
    return true;
}

/* External linkage as required by the build. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1. */
    if (n == 0)
        return 0;   /* undefined, but avoid UB */

    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (is_allowed(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
