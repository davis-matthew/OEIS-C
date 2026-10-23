/*
 * A192863  Lower flat numbers: odd numbers k such that k-1 is a squarefree
 *          number times a power of two.
 * https://oeis.org/A192863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer k such that (k-1)/2^v₂(k-1) is
 *            square‑free (OEIS A192863)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* Check whether an odd integer k satisfies the lower‑flat condition. */
static bool is_lower_flat(seq_t k)
{
    /* only odd k are allowed */
    if ((k & (seq_t)1) == 0)
        return false;

    /* remove all factors of 2 from k-1 */
    seq_t m = k - (seq_t)1;
    while ((m & (seq_t)1) == 0)
        m >>= 1;

    /* the remaining part must be square‑free */
    return is_squarefree(m);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for the offset 1 */

    /* Start at the first admissible odd integer. */
    seq_t candidate = (seq_t)3;
    idx_t count = 1;              /* candidate is the first term */

    while (count < n) {
        candidate += (seq_t)2;    /* next odd integer */
        if (is_lower_flat(candidate))
            ++count;
    }

    return candidate;
}
